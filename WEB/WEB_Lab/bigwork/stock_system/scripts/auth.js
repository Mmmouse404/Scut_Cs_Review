// 注册功能
document.getElementById('registerForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('regUsername').value;
    const password = document.getElementById('regPassword').value;
    const confirm = document.getElementById('regConfirm').value;

    if (password !== confirm) {
        alert('两次密码输入不一致');
        return;
    }

    try {
        const response = await fetch(`http://localhost:12345/regist?username=${encodeURIComponent(username)}&pwd=${encodeURIComponent(password)}`);
        const result = await response.text();
        if (result === 'true') {
            alert('注册成功');
            window.location.reload();
        } else {
            alert('注册失败：用户名已存在或格式错误');
        }
    } catch (error) {
        alert('请求失败: ' + error.message);
    }
});

// 登录功能
document.getElementById('loginForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    const username = document.getElementById('loginUsername').value;
    const password = document.getElementById('loginPassword').value;

    try {
        const response = await fetch(`http://localhost:12345/login?username=${encodeURIComponent(username)}&pwd=${encodeURIComponent(password)}`);
        if (await response.text() === 'true') {
            sessionStorage.setItem('username', username);
            window.location.href = 'user.html';
        } else {
            alert('登录失败：用户名或密码错误');
        }
    } catch (error) {
        alert('请求失败: ' + error.message);
    }
});

