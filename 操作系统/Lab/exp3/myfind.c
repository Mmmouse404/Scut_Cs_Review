#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <regex.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0
#define MAX_COUNT 500 // 查询结果存放上界

int nameFlag = FALSE; // 是否按名搜索
int sizeFlag = FALSE; // 是否按大小搜索

int nameIndex = 0; // -name 是第几个参数
int sizeIndex = 0; // -size 是第几个参数

char* result[MAX_COUNT]; // 存储查询结果
int resultIndex = 0; // result 数组下标

void printHelp()
{
    printf("用法：./myfind 待查找路径 选项 查找条件\n");
    printf("选项说明：\n");
    printf("-name \"文件\"\n\t\"指定要查找的文件名，支持使用通配符模糊搜索\"\n");
    printf("-size min_size:max_size\n\t\"搜索特定大小或特定大小范围内的文件（默认单位为字节）\"\n");
}

void init(int argc, char* argv[])
{
    int i;
    if (argv[2][0] != '-') {
        printHelp();
        exit(EXIT_FAILURE);
    }
    for (i = 2; i < argc; i++) {
        if (argv[i][0] != '-')
            continue;
        if (strcmp(argv[i], "-name") == 0) {
            nameFlag = TRUE;
            nameIndex = i;
        } else if (strcmp(argv[i], "-size") == 0) {
            sizeFlag = TRUE;
            sizeIndex = i;
        }
    }
}

int matchName(char* string, char* pattern)
{
    regex_t reg;
    if (regcomp(&reg, pattern, REG_NOSUB | REG_ICASE | REG_EXTENDED) != 0) {
        perror("compile regex error");
        printf("regex:%s\n", pattern);
        exit(EXIT_FAILURE);
    }
    int status = regexec(&reg, string, (size_t)0, NULL, 0);
    regfree(&reg);
    if (status == 0)
        return TRUE;
    return FALSE;
}

int matchSize(struct stat stat_buf, char* sizeStr)
{
    long minSize, maxSize;
    char* colon = strchr(sizeStr, ':');
    if (colon) {
        *colon = '\0';
        minSize = atol(sizeStr);
        maxSize = atol(colon + 1);
        *colon = ':'; // Restore the string
    } else {
        minSize = atol(sizeStr);
        maxSize = minSize;
    }
    if (stat_buf.st_size >= minSize && stat_buf.st_size <= maxSize)
        return TRUE;
    return FALSE;
}

char* getPath(char* path, char* filename)
{
    int len1 = strlen(path);
    int len2 = strlen(filename);
    char* str = (char*)malloc(len1 + len2 + 2);
    str[0] = '\0';
    str = strcat(str, path);
    if (path[len1 - 1] != '/') str = strcat(str, "/");
    str = strcat(str, filename);
    return str;
}

void myfind(int argc, char* argv[], char* path)
{
    DIR* dir;
    struct dirent* doc;
    struct stat stat_buf;
    if (path == NULL) path = argv[1];
    char* filepath;

    if ((dir = opendir(path)) == NULL) {
        perror("function opendir:");
        printf("dir:%s\n", path);
        return;
    }
    do {
        errno = 0;
        if ((doc = readdir(dir)) != NULL) {
            int matchFlag = TRUE;
            filepath = getPath(path, doc->d_name);
            if (lstat(filepath, &stat_buf) != 0) {
                perror("function stat error");
                printf("stat path:%s\n", filepath);
                continue;
            }
            if (nameFlag == TRUE) {
                int len = strlen(argv[nameIndex + 1]);
                if (argv[nameIndex + 1][0] == '"' && argv[nameIndex + 1][len - 1] == '"') {
                    argv[nameIndex + 1][len - 1] = '\0';
                    argv[nameIndex + 1]++;
                }
                if (!matchName(doc->d_name, argv[nameIndex + 1]))
                    matchFlag = FALSE;
            }
            if (sizeFlag == TRUE) {
                if (!matchSize(stat_buf, argv[sizeIndex + 1]))
                    matchFlag = FALSE;
            }
            if (matchFlag == TRUE) {
                if (resultIndex < 500)
                    result[resultIndex++] = filepath;
                printf("%s\n", filepath);
            }
            if (S_ISDIR(stat_buf.st_mode) && strcmp(doc->d_name, ".") != 0 && strcmp(doc->d_name, "..") != 0) {
                myfind(argc, argv, filepath);
            }
        }
    } while (doc != NULL);
    if (errno != 0) {
        perror("readdir error");
        exit(EXIT_FAILURE);
    }
    closedir(dir);
}

int main(int argc, char* argv[])
{
    if (argc <= 2) {
        printf("格式错误!\n");
        printHelp();
        exit(EXIT_FAILURE);
    }
    init(argc, argv);
    myfind(argc, argv, argv[1]);
    return EXIT_SUCCESS;
}
