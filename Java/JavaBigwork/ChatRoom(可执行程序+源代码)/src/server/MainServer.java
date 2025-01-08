package server;

import server.controller.RequestProcessor;
import server.ui.ServerInfoFrame;

import javax.swing.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;


public class MainServer {
    static String serverIp;
    private static void updateServerIpInProperties(String serverIp) {
        try {
            // 获取项目根目录
            String rootPath = System.getProperty("user.dir");
            // 构造配置文件路径
            String configFilePath = rootPath + "/src/serverconfig.properties";
            System.out.println("配置文件路径: " + configFilePath);
            // 检查文件是否存在
            File configFile = new File(configFilePath);
            if (!configFile.exists()) {
                throw new FileNotFoundException("配置文件未找到: " + configFilePath);
            }

            // 读取文件内容
            List<String> lines = new ArrayList<>();
            try (BufferedReader reader = new BufferedReader(new FileReader(configFilePath))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    lines.add(line);
                }
            }
            // 更新IP地址
            boolean ipUpdated = false;
            for (int i = 0; i < lines.size(); i++) {
                if (lines.get(i).startsWith("ip=")) {
                    lines.set(i, "ip=" + serverIp);
                    ipUpdated = true;
                    break;
                }
            }

            // 如果没有找到IP配置项，添加一行
            if (!ipUpdated) {
                lines.add("ip=" + serverIp);
            }

            // 将更新后的内容写回文件
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(configFilePath))) {
                for (String line : lines) {
                    writer.write(line);
                    writer.newLine();
                }
            }
            System.out.println("配置文件更新成功，IP 地址: " + serverIp);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    public static void main(String[] args) {
        int port = Integer.parseInt(DataBuffer.configProp.getProperty("port"));
        //初始化服务器套节字
        try {
            DataBuffer.serverSocket = new ServerSocket(port);
            serverIp = InetAddress.getLocalHost().getHostAddress();
            System.out.println("服务器已启动，IP 地址: " + serverIp);
            // 更新配置文件中的IP地址
            updateServerIpInProperties(serverIp);
        } catch (IOException e) {
            e.printStackTrace();
        }

        new Thread(new Runnable() {//启动新线程进行客户端连接监听
            public void run() {
                try {
                    while (true) {
                        // 监听客户端的连接
                        Socket socket = DataBuffer.serverSocket.accept();
                        System.out.println("客户来了："
                                + socket.getInetAddress().getHostAddress()
                                + ":" + socket.getPort());

                        //针对每个客户端启动一个线程，在线程中调用请求处理器来处理每个客户端的请求
                        new Thread(new RequestProcessor(socket)).start();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();

        //设置外观感觉
        JFrame.setDefaultLookAndFeelDecorated(true);
        JDialog.setDefaultLookAndFeelDecorated(true);
        try {
            UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
        } catch (Exception e) {
            e.printStackTrace();
        }
        //启动服务器监控窗体
        new ServerInfoFrame(serverIp);
    }
}
