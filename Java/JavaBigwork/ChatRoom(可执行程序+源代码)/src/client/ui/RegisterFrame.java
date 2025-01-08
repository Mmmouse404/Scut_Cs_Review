package client.ui;

import client.util.ClientUtil;
import common.model.entity.Request;
import common.model.entity.Response;
import common.model.entity.ResponseStatus;
import common.model.entity.User;
import server.DataBuffer;

import javax.swing.*;
import java.awt.*;
import javax.imageio.ImageIO;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.File;
import java.io.IOException;


public class RegisterFrame extends JFrame {
    private static final long serialVersionUID = -768631070458723803L;
    private JPasswordField pwdFld;
    private JPasswordField pwd2Fld;
    private JTextField nickname;
    private JComboBox<ImageIcon> head;
    private JRadioButton sex0;
    private JRadioButton sex1;
    private JButton ok;
    private JButton reset;
    private JButton cancel;

    public RegisterFrame(){
        this.init();
        setVisible(true);
    }

    public void init(){
        this.setTitle("注册新账号");//设置标题
        setBounds((DataBuffer.screenSize.width - 387)/2,
                (DataBuffer.screenSize.height - 267)/2,
                387, 267);
        getContentPane().setLayout(null);
        setResizable(false);

        JLabel label = new JLabel("昵称:"); //label显示
        label.setBounds(24, 36, 59, 17);
        getContentPane().add(label);

        nickname = new JTextField(); //昵称
        nickname.setBounds(90, 34, 110, 22);
        getContentPane().add(nickname);

        JLabel label5 = new JLabel("密码:*");
        label5.setBounds(24, 72, 50, 17);
        getContentPane().add(label5);

        JLabel label3 = new JLabel("确认密码:*");
        label3.setBounds(24, 107, 65, 17);
        getContentPane().add(label3);

        pwdFld = new JPasswordField();//密码框
        pwdFld.setBounds(90, 70, 110, 22);
        getContentPane().add(pwdFld);

        pwd2Fld = new JPasswordField();
        pwd2Fld.setBounds(90, 105, 110, 22);
        getContentPane().add(pwd2Fld);

        JLabel label4 = new JLabel("性别:");
        label4.setBounds(230, 36, 31, 17);
        getContentPane().add(label4);

        sex1 = new JRadioButton("男",true);
        sex1.setBounds (268, 31,44, 25);
        getContentPane().add(sex1);
        sex0 = new JRadioButton("女");
        sex0.setBounds(310, 31, 44, 25);
        getContentPane().add(sex0);
        ButtonGroup buttonGroup = new ButtonGroup();//单选按钮组
        buttonGroup.add(sex0);
        buttonGroup.add(sex1);

        JLabel label6 = new JLabel("头像:");
        label6.setBounds(230, 72, 31, 17);
        getContentPane().add(label6);

        head = new JComboBox();//下拉列表图标
        head.setBounds(278, 70, 65, 45);
        head.setMaximumRowCount(5);
        for (int i = 0; i < 11; i++) {
            head.addItem(new ImageIcon("images/" + i + ".png"));
            //通过循环添加图片，注意图片名字要取成1,2,3,4,5,等
        }
        head.setSelectedIndex(0);
        getContentPane().add(head);
        // 添加头像上传按钮
        JButton uploadButton = new JButton("上传头像");
        uploadButton.setBounds(240, 120, 100, 28);
        getContentPane().add(uploadButton);

        // 添加上传按钮的事件监听
        uploadButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser fileChooser = new JFileChooser();
                fileChooser.setDialogTitle("选择头像文件");
                fileChooser.setFileFilter(new javax.swing.filechooser.FileNameExtensionFilter("图片文件",
                        "jpg", "jpeg", "png", "gif"));

                int result = fileChooser.showOpenDialog(RegisterFrame.this);
                if (result == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = fileChooser.getSelectedFile();

                    // 生成新的文件名
                    String newFileName = generateNewFileName();

                    // 保存文件到 images 目录
                    try {
                        File destFile = new File("images/" + newFileName);
                        // 将文件复制到 images 文件夹
                        BufferedImage originalImage = ImageIO.read(selectedFile);
                        BufferedImage resizedImage = resizeImage(originalImage, 40, 40); // 假设头像框宽和高都是100像素
                        ImageIO.write(resizedImage, "png", destFile); // 可以选择jpg或其他格式
                        // 更新下拉框头像列表
                        head.addItem(new ImageIcon(destFile.getAbsolutePath())); // 添加新选择的头像
                        head.setSelectedItem(new ImageIcon(destFile.getAbsolutePath())); // 设置为默认选择

                        JOptionPane.showMessageDialog(RegisterFrame.this, "头像上传成功!", "成功", JOptionPane.INFORMATION_MESSAGE);
                    } catch (IOException ioException) {
                        JOptionPane.showMessageDialog(RegisterFrame.this, "头像上传失败: " + ioException.getMessage(), "上传失败", JOptionPane.ERROR_MESSAGE);
                    }
                }
            }
        });

        //按钮
        ok = new JButton("确认");
        ok.setBounds(27, 176, 60, 28);
        getContentPane().add(ok);

        reset = new JButton("重填");
        reset.setBounds(123, 176, 60, 28);
        getContentPane().add(reset);

        cancel = new JButton("取消");
        cancel.setBounds(268, 176, 60, 28);
        getContentPane().add(cancel);

        //////////////////////注册事件监听器////////////////////////
        //取消按钮监听事件处理
        cancel.addActionListener(new ActionListener() {
            public void actionPerformed(final ActionEvent event) {
                RegisterFrame.this.dispose();
            }
        });
        //关闭窗口
        this.addWindowListener(new WindowAdapter(){
            public void windowClosing(WindowEvent e) {
                RegisterFrame.this.dispose();
            }
        });

        // 重置按钮监听事件处理
        reset.addActionListener(new ActionListener() {
            public void actionPerformed(final ActionEvent e) {
                nickname.setText("");
                pwdFld.setText("");
                pwd2Fld.setText("");
                nickname.requestFocusInWindow();//用户名获得焦点
            }
        });

        //确认按钮监听事件处理
        ok.addActionListener(new ActionListener() {
            public void actionPerformed(final ActionEvent e) {
                if (pwdFld.getPassword().length==0 || pwd2Fld.getPassword().length==0) {
                    JOptionPane.showMessageDialog(RegisterFrame.this, "带 “ * ” 为必填内容!");
                    //判断用户名和密码是否为空
                } else if (!new String(pwdFld.getPassword()).equals(new String(pwd2Fld.getPassword()))) {
                    JOptionPane.showMessageDialog(RegisterFrame.this, "两次输入密码不一致!");
                    pwdFld.setText("");
                    pwd2Fld.setText("");
                    pwdFld.requestFocusInWindow();
                    //判断两次密码是否一致
                } else {
                    User user = new User(new String(pwdFld.getPassword()),
                            nickname.getText(),
                            sex0.isSelected() ? 'm' : 'f',
                            head.getSelectedIndex());
                    try {
                        RegisterFrame.this.register(user);
                    } catch (IOException | ClassNotFoundException e1) {
                        e1.printStackTrace();
                    }
                }
            }
        });
    }
    private String generateNewFileName() {
        File imagesDir = new File("images");
        int fileIndex = 0;

        // 查找可用的自增编号
        while (true) {
            String fileName = fileIndex + ".png"; // 新文件名格式
            File tempFile = new File(imagesDir, fileName);
            if (!tempFile.exists()) { // 如果文件不存在，就找到了可用的名称
                return fileName;
            }
            fileIndex++; // 自增下一个编号
        }
    }
    private BufferedImage resizeImage(BufferedImage originalImage, int targetWidth, int targetHeight) {
        Image resultingImage = originalImage.getScaledInstance(targetWidth, targetHeight, Image.SCALE_SMOOTH);
        BufferedImage outputImage = new BufferedImage(targetWidth, targetHeight, BufferedImage.TYPE_INT_ARGB);

        Graphics2D g2d = outputImage.createGraphics();
        g2d.drawImage(resultingImage, 0, 0, null);
        g2d.dispose();

        return outputImage;
    }
    //注册方法
    private void register(User user) throws IOException, ClassNotFoundException{
        Request request = new Request();
        request.setAction("userRegister");
        request.setAttribute("user", user);

        //获取响应
        Response response = ClientUtil.sendTextRequest(request);

        ResponseStatus status = response.getStatus();
        if (status == ResponseStatus.OK) {
            User user2 = (User) response.getData("user");
            JOptionPane.showMessageDialog(RegisterFrame.this,
                    "恭喜您，您的Q号:" + user2.getId() + ",请牢记!!!",
                    "注册成功", JOptionPane.INFORMATION_MESSAGE);
            this.setVisible(false);
        } else {
            JOptionPane.showMessageDialog(RegisterFrame.this,
                    "注册失败，请稍后再试！！！", "服务器内部错误！", JOptionPane.ERROR_MESSAGE);
        }
    }
}
