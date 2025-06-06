package client.ui;

import client.DataBuffer;
import client.model.entity.MatesListModel;
import client.util.ClientUtil;
import common.model.entity.Request;
import common.model.entity.Response;
import common.model.entity.ResponseStatus;
import common.model.entity.User;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import static server.DataBuffer.matesList;


public class LoginFrame extends JFrame {
    private static final long serialVersionUID = -3426717670093483287L;
    private JTextField ipTxt; // 添加一个字段用于输入IP
    private JTextField idTxt;
    private JPasswordField pwdFld;

    public LoginFrame(){
        this.init();
        setVisible(true);
    }

    public void init(){
        this.setTitle("用户登录");
        this.setSize(400, 250);
        //设置默认窗体在屏幕中央
        int x = (int) Toolkit.getDefaultToolkit().getScreenSize().getWidth();
        int y = (int)Toolkit.getDefaultToolkit().getScreenSize().getHeight();
        this.setLocation((x - this.getWidth()) / 2, (y-this.getHeight())/ 2);
        this.setResizable(false);

        //把Logo放置到JFrame的北边
        ImageIcon icon = new ImageIcon("images/mouse.png");
        Icon scaledIcon = new ImageIcon(icon.getImage().getScaledInstance(50, 50, Image.SCALE_SMOOTH));
        JLabel label = new JLabel(scaledIcon);
        label.setPreferredSize(new Dimension(324,47));
        this.add(label, BorderLayout.NORTH);

        //登录信息面板
        JPanel mainPanel = new JPanel();
        Border border = BorderFactory.createEtchedBorder(EtchedBorder.LOWERED);
        mainPanel.setBorder(BorderFactory.createTitledBorder(border, "输入登录信息", TitledBorder.CENTER,TitledBorder.TOP));
        this.add(mainPanel, BorderLayout.CENTER);
        mainPanel.setLayout(null);
    // 在此处添加 IP 输入框
        JLabel ipLbl = new JLabel("IP地址:");
        ipLbl.setBounds(50, 100, 60, 22);
        mainPanel.add(ipLbl);
        String defaultIP=DataBuffer.configProp.getProperty("ip");
        ipTxt = new JTextField(defaultIP); // 默认IP为localhost
        ipTxt.setBounds(95, 100, 150, 22);
        mainPanel.add(ipTxt);

        JLabel nameLbl = new JLabel("账号:");
        nameLbl.setBounds(50, 30, 40, 22);
        mainPanel.add(nameLbl);
        idTxt = new JTextField();
        idTxt.setBounds(95, 30, 150, 22);
        idTxt.requestFocusInWindow();//用户名获得焦点
        mainPanel.add(idTxt);

        JLabel pwdLbl = new JLabel("密码:");
        pwdLbl.setBounds(50, 60, 40, 22);
        mainPanel.add(pwdLbl);
        pwdFld = new JPasswordField();
        pwdFld.setBounds(95, 60, 150, 22);
        mainPanel.add(pwdFld);

        //按钮面板放置在JFrame的南边
        JPanel btnPanel = new JPanel();
        this.add(btnPanel, BorderLayout.SOUTH);
        btnPanel.setLayout(new BorderLayout());
        btnPanel.setBorder(new EmptyBorder(2, 8, 4, 8));

        JButton registerBtn = new JButton("注册");
        btnPanel.add(registerBtn, BorderLayout.WEST);
        JButton submitBtn = new JButton("登录");
        btnPanel.add(submitBtn, BorderLayout.EAST);

        //关闭窗口
        this.addWindowListener(new WindowAdapter(){
            public void windowClosing(WindowEvent e) {
                Request req = new Request();
                req.setAction("exit");
                try {
                    ClientUtil.sendTextRequest(req);
                } catch (IOException ex) {
                    ex.printStackTrace();
                }finally{
                    System.exit(0);
                }
            }
        });

        //注册
        registerBtn.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                new RegisterFrame();  //打开注册窗体
            }
        });

        //"登录"
        submitBtn.addActionListener(new ActionListener(){
            public void actionPerformed(ActionEvent e) {

                login();
            }
        });
    }

    /** 登录 */
    @SuppressWarnings("unchecked")
    private void login() {
        if (idTxt.getText().length() == 0
                || pwdFld.getPassword().length == 0) {
            JOptionPane.showMessageDialog(LoginFrame.this,
                    "账号和密码是必填的",
                    "输入有误",JOptionPane.ERROR_MESSAGE);
            idTxt.requestFocusInWindow();
            return;
        }

        if(!idTxt.getText().matches("^\\d*$")){
            JOptionPane.showMessageDialog(LoginFrame.this,
                    "账号必须是数字",
                    "输入有误",JOptionPane.ERROR_MESSAGE);
            idTxt.requestFocusInWindow();
            return;
        }

        Request req = new Request();
        req.setAction("userLogin");
        req.setAttribute("id", idTxt.getText());
        req.setAttribute("password", new String(pwdFld.getPassword()));

        //获取响应
        Response response = null;
        try {
            response = ClientUtil.sendTextRequest(req);
        } catch (IOException e1) {
            e1.printStackTrace();
        }

        if(response.getStatus() == ResponseStatus.OK){
            //获取当前用户
            User user2 = (User)response.getData("user");
            if(user2!= null){ //登录成功
                DataBuffer.currentUser = user2;

                //获取当前在线用户列表
                DataBuffer.onlineUsers = (List<User>)response.getData("onlineUsers");

                if(matesList.size() < 100) {
                    for (int i = 0; i < 100; i++) {
                        DataBuffer.matesList.add(new ArrayList<User>());
                        DataBuffer.matesListModels.add(new MatesListModel());
                    }
                }
                LoginFrame.this.dispose();
                new ChatFrame();  //打开聊天窗体
            }else{ //登录失败
                String str = (String)response.getData("msg");
                JOptionPane.showMessageDialog(LoginFrame.this,
                        str,
                        "登录失败",JOptionPane.ERROR_MESSAGE);
            }
        }else{
            JOptionPane.showMessageDialog(LoginFrame.this,
                    "服务器内部错误，请稍后再试！！！","登录失败",JOptionPane.ERROR_MESSAGE);
        }
    }
    public String getIp() {
        return ipTxt.getText();
    }

}
