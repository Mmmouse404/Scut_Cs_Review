package client;

import client.ui.LoginFrame;

import javax.swing.*;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;



public class ClientMain {

    public static void main(String[] args) {

        //设置外观感觉
        JFrame.setDefaultLookAndFeelDecorated(true);
        JDialog.setDefaultLookAndFeelDecorated(true);
        try {
            //UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
            String lookAndFeel = UIManager.getSystemLookAndFeelClassName();
            UIManager.setLookAndFeel(lookAndFeel);
//            UIManager.setLookAndFeel(new SubstanceBusinessLookAndFeel());

        } catch (Exception e) {
            e.printStackTrace();
        }

        LoginFrame loginFrame = new LoginFrame();  //启动登录窗体
        connection(loginFrame); //连接到服务器
    }

    /** 连接到服务器 */
    public static void connection(LoginFrame loginFrame) {
        //String ip = DataBuffer.configProp.getProperty("ip");
        String ip = loginFrame.getIp(); // 从登录窗体获取IP
        int port = Integer.parseInt(DataBuffer.configProp.getProperty("port"));
        try {
            DataBuffer.clientSeocket = new Socket(ip, port);
            DataBuffer.oos = new ObjectOutputStream(DataBuffer.clientSeocket.getOutputStream());
            DataBuffer.ois = new ObjectInputStream(DataBuffer.clientSeocket.getInputStream());

        } catch (Exception e) {
            JOptionPane.showMessageDialog(new JFrame(),
                    "连接服务器失败,请检查!","服务器未连上", JOptionPane.ERROR_MESSAGE);//否则连接失败
            System.exit(0);
        }
    }
}
