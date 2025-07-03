// scripts/trade.js
let refreshTimer;
//let stockChart = null;
const stateMap = {
    0: '系统错误',
    1: '委托成功',
    2: '交易成功',
    3: '废单',
    4: '余额不足',
    5: '持仓不足'
};
// 注销功能
document.addEventListener('DOMContentLoaded', () => {
    const logoutBtn = document.getElementById('logoutBtn');
    if (logoutBtn) {
        logoutBtn.addEventListener('click', async () => {
            const username = sessionStorage.getItem('username');
            try {
                await fetch(`http://localhost:12345/logout?username=${encodeURIComponent(username)}`);
            } catch (error) {
                console.error('注销请求失败:', error);
            }
            sessionStorage.removeItem('username');
            window.location.href = 'index.html';
        });
    }
});
// 股票代码输入时自动获取价格
document.getElementById('tradeCode').addEventListener('input', async (e) => {
    const code = e.target.value.trim();
    const display = document.getElementById('priceDisplay');
    
    // 清除旧定时器
    if(refreshTimer) {
        clearInterval(refreshTimer);
        refreshTimer = null;
    }

    if (code.length === 6) {
        // 立即获取并显示
        const initialPrice = await getCurrentPrice(code);
        updatePriceDisplay(display, initialPrice);
        
        // 启动新定时器（与服务端同步5秒）
        refreshTimer = setInterval(async () => {
            const newPrice = await getCurrentPrice(code);
            updatePriceDisplay(display, newPrice);
        }, 5000);
    } else {
        display.textContent = '';
    }
});
// 初始化交易模块
document.addEventListener('DOMContentLoaded', () => {
    const username = sessionStorage.getItem('username');
    if (!username) window.location.href = 'index.html';

    document.getElementById('usernameDisplay').textContent = username;
    loadAllData();
    setupAutoRefresh();
});

// 加载所有数据
async function loadAllData() {
    try {
        await Promise.all([
            updateBalance(),
            fetchPositions(),
            fetchRecords()
        ]);
    } catch (error) {
        showError('数据加载失败', error);
    }
}

// 交易表单提交
document.getElementById('tradeForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    // 获取实时价格
    const code = document.getElementById('tradeCode').value.trim();
    const realTimePrice = await getCurrentPrice(code);

    if (!realTimePrice) {
        showMessage('无法获取实时行情');
        return;
    }
    const tradeData = {
        code: document.getElementById('tradeCode').value.trim(),
        direction: document.getElementById('tradeDirection').value,
        amount: parseInt(document.getElementById('tradeAmount').value),
        //price: parseFloat(document.getElementById('tradePrice').value),
        price: realTimePrice,
        username: sessionStorage.getItem('username')
    };

    if (!validateTrade(tradeData)) return;

    try {
        const result = await submitTrade(tradeData);
        showMessage(`交易结果：${stateMap[result]}`);
        loadAllData(); // 刷新所有数据
    } catch (error) {
        showError('交易请求失败', error);
    }
});
// 新增获取实时价格方法
async function getCurrentPrice(code) {
    try {
        const response = await fetch('http://localhost:12345/getMarketPrice');
        const marketData = await response.json();
        const stock = marketData.find(s => s.Code === code);
        return stock?.Price || null;
    } catch (error) {
        console.error('获取实时价格失败:', error);
        return null;
    }
}
// 提交交易请求
async function submitTrade(data) {
    const params = new URLSearchParams({
        username: data.username,
        code: data.code,
        direction: data.direction,
        price: data.price,
        amount: data.amount
    });

    const response = await fetch(`http://localhost:12345/trade?${params}`);
    if (!response.ok) throw new Error(`HTTP错误 ${response.status}`);
    return parseInt(await response.text());
}

/// 验证逻辑
function validateTrade(data) {
    if (data.amount % 100 !== 0) {
        showMessage('交易数量必须是100的整数倍');
        return false;
    }
    if (!data.code.match(/^(6|3|0)\d{5}$/)) {
        showMessage('股票代码格式错误');
        return false;
    }
    if (!data.price) {
        showMessage('获取实时价格失败');
        return false;
    }
    return true;
}

// 持仓加载
async function fetchPositions() {
    const username = sessionStorage.getItem('username');
    try {
        // 同时获取持仓数据和市场数据
        const [positions, marketData] = await Promise.all([
            fetch(`http://localhost:12345/getInventory?username=${username}`).then(res => res.json()),
            fetch('http://localhost:12345/getMarketPrice').then(res => res.json())
        ]);

        const tbody = document.querySelector('#positionTable tbody');
        tbody.innerHTML = positions.map(pos => {
            // 匹配市场数据中的股票信息
            const stock = marketData.find(s => s.Code === pos.Code);
            const currentPrice = stock ? stock.Price : 0;
            // 计算实时盈亏
            const profit = (currentPrice - pos.AVG_Cost) * pos.Amount;

            return `
                <tr data-code="${pos.Code}">
                    <td>${pos.Code}</td>
                    <td>${stock?.Name || '--'}</td>
                    <td>${pos.Amount}</td>
                    <td class="${profit >= 0 ? 'profit' : 'loss'}">
                        ${profit.toFixed(2)}
                    </td>
                </tr>
            `;
        }).join('');

        // 添加持仓股票点击事件
        document.querySelectorAll('#positionTable tbody tr').forEach(row => {
            row.addEventListener('click', (e) => {
                const code = row.dataset.code;
                // 调用显示操作菜单的函数
                this.showActionMenu(code, e);
            });
        });
    } catch (error) {
        showError('持仓加载失败', error);
    }
}


// 获取交易记录（添加状态列）
async function fetchRecords() {
    const username = sessionStorage.getItem('username');
    try {
        const response = await fetch(`http://localhost:12345/getTradeRecord?username=${username}`);
        const data = await response.json();
        const tbody = document.querySelector('#recordTable tbody');
        tbody.innerHTML = data.map(record => `
            <tr>
                <td>${record.TradeTime}</td>
                <td>${record.Code}</td>
                <td class="${record.Direction === 0 ? 'buy' : 'sell'}">${record.Direction === 0 ? '买入' : '卖出'}</td>
                <td>${record.Amount}</td>
                <td>${record.Price?.toFixed(2) || '--'}</td>
                <td>${stateMap[record.State] || '未知'}</td>
            </tr>
        `).join('');
    } catch (error) {
        showError('交易记录加载失败', error);
    }
}
function destroyChart() {
    window.stockChart.destroy();
  }

// 个股走势图逻辑
function loadStockChart(code) {
    fetch(`http://localhost:12345/getStockPrice?code=${code}`)
        .then(response => {
            if (!response.ok) throw new Error('请求失败');
            return response.json();
        })
        .then(prices => {
            const ctx = document.getElementById('stockChart').getContext('2d');
            const latestPrices = prices.slice(-100);
            // 销毁旧图表（安全写法）
            if (stockChart instanceof Chart) {
                stockChart.destroy();
            }

            // 创建新图表
            stockChart = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: prices.map((_, i) => `T${i + 1}`).slice(-100),
                    datasets: [{
                        label: `${code} 价格走势`,
                        data: latestPrices,
                        borderColor: '#2196F3',
                        fill: false,
                        tension: 0.1
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    scales: {
                        y: { beginAtZero: false }
                    }
                }
            });
        })
        .catch(error => {
            console.error('图表加载失败:', error);
            showMessage('无法加载历史数据');
        });
}
// 更新余额
async function updateBalance() {
    const username = sessionStorage.getItem('username');
    const response = await fetch(`http://localhost:12345/getBalance?username=${username}`);
    const balance = await response.json();
    document.getElementById('balance').textContent = `${balance.toFixed(2)}元`;
}

// 自动刷新设置
function setupAutoRefresh() {
    refreshTimer = setInterval(() => {
        loadAllData();
    }, 5000); // 每5秒刷新
}

// 统一更新价格显示
function updatePriceDisplay(displayElement, price) {
    if (!price) {
        displayElement.textContent = '代码无效';
        displayElement.style.color = '#ff4444';
    } else {
        const prevPrice = parseFloat(displayElement.dataset.lastPrice) || 0;
        displayElement.textContent = `当前价格：${price.toFixed(2)}`;
        displayElement.dataset.lastPrice = price;
        
        // 价格变化动画
        displayElement.style.color = price > prevPrice ? '#ff4444' : 
                                    price < prevPrice ? '#4CAF50' : '#666';
        displayElement.classList.add('price-update');
        setTimeout(() => {
            displayElement.classList.remove('price-update');
        }, 500);
    }
}
// 页面卸载时清理
window.addEventListener('beforeunload', () => {
    clearInterval(refreshTimer);
});

// 工具函数
function formatTime(timestamp) {
    return new Date(timestamp).toLocaleString();
}

function showMessage(msg) {
    const el = document.createElement('div');
    el.className = 'alert';
    el.textContent = msg;
    document.body.appendChild(el);
    setTimeout(() => el.remove(), 3000);
}

function showError(title, error) {
    console.error(`${title}:`, error);
    showMessage(`${title}: ${error.message}`);
}




let chartUpdateInterval = null; // 图表更新定时器
//绘制个股走势图
function createBeautifulChart(code, prices) {
    const ctx = document.getElementById('stockChart').getContext('2d');
    
    // 销毁旧图表
    if (window.stockChart instanceof Chart) {
        window.stockChart.destroy();
    }
    const latestPrices = prices.slice(-20);
    // 高级配置选项
    const gradient = ctx.createLinearGradient(0, 0, 0, 400);
    gradient.addColorStop(0, 'rgba(33,150,243,0.8)');
    gradient.addColorStop(1, 'rgba(33,150,243,0.1)');

    // 创建新图表
    window.stockChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: generateTimeLabels(prices.length),
            datasets: [{
                label: `${code} 价格走势`,
                data: latestPrices,
                backgroundColor: gradient,
                borderColor: '#2196F3',
                borderWidth: 2,
                pointRadius: 3,
                pointHoverRadius: 6,
                fill: true,
                tension: 0.4
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            animation: {
                duration: 2000,
                easing: 'easeOutQuart'
            },
            plugins: {
                legend: {
                    display: true,
                    position: 'top',
                    labels: {
                        color: '#666',
                        font: {
                            size: 14
                        }
                    }
                },
                tooltip: {
                    backgroundColor: 'rgba(0,0,0,0.8)',
                    titleFont: { size: 16 },
                    bodyFont: { size: 14 },
                    padding: 12,
                    usePointStyle: true
                }
            },
            scales: {
                x: {
                    grid: {
                        color: 'rgba(0,0,0,0.05)'
                    },
                    ticks: {
                        color: '#666',
                        maxTicksLimit: 10,
                        autoSkip: true
                    }
                },
                y: {
                    grid: {
                        color: 'rgba(0,0,0,0.05)'
                    },
                    ticks: {
                        color: '#666',
                        callback: function(value) {
                            return '¥' + value.toFixed(2);
                        }
                    }
                }
            }
        }
    });
}

// 生成时间标签（每5分钟一个点）
function generateTimeLabels(dataLength) {
    return Array.from({length: dataLength}, (_, i) => {
        const date = new Date(Date.now() - (dataLength - i - 1) * 500000); // 5分钟间隔
        return date.toLocaleTimeString([], {hour: '2-digit', minute: '2-digit'});
    });
}

// 更新频率控制（30秒更新一次）
function setupChartAutoRefresh(code) {
    // 清除旧定时器
    if (chartUpdateInterval) clearInterval(chartUpdateInterval);
    
    // 设置新定时器
    chartUpdateInterval = setInterval(async () => {
        try {
            const prices = await fetchStockPrices(code);
            window.stockChart.data.datasets[0].data = prices;
            window.stockChart.data.labels = generateTimeLabels(prices.length);
            window.stockChart.update();
        } catch (error) {
            console.error('图表更新失败:', error);
        }
    }, 30000); // 30秒间隔
}

//显示买入、卖出
function showMessage(msg) {
    const el = document.createElement('div');
    el.className = 'alert';
    el.textContent = msg;
    document.body.appendChild(el);
    setTimeout(() => el.remove(), 3000);
}
// 获取股票价格（带缓存）
let lastFetchTime = 0;
async function fetchStockPrices(code) {
    // 5秒请求缓存
    if (Date.now() - lastFetchTime < 5000) {
        return window.stockChart.data.datasets[0].data;
    }
    
    const response = await fetch(`http://localhost:12345/getStockPrice?code=${code}`);
    lastFetchTime = Date.now();
    return response.json();
}
// 清空个股走势图表
function clearStockChart() {
    if (window.stockChart instanceof Chart) {
        // 销毁图表
        window.stockChart.destroy();
        
        // 清空 canvas
        const ctx = document.getElementById('stockChart').getContext('2d');
        ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
        
        // 创建一个空数据的图表
        window.stockChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: [],
                datasets: [{
                    label: '',
                    data: [],
                    borderColor: '#2196F3',
                    fill: false,
                    tension: 0.1
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: { beginAtZero: false }
                }
            }
        });
    }
}
function incrementAmount() {
    const input = document.getElementById('tradeAmount');
    const currentValue = parseInt(input.value) || 0;
    input.value = currentValue + 100;
}

function decrementAmount() {
    const input = document.getElementById('tradeAmount');
    const currentValue = parseInt(input.value) || 0;
    input.value = Math.max(currentValue - 100, 0); // 确保值不小于0
}
// 显示操作菜单
function showActionMenu(code, e) {
    // 移除现有的菜单
    const existingMenu = document.querySelector('.action-menu');
    if (existingMenu) {
        existingMenu.remove();
    }
    // 创建菜单容器
    const menu = document.createElement('div');
    menu.className = 'action-menu';
    menu.innerHTML = `
        <div class="menu-item" data-action="buy">买入</div>
        <div class="menu-item" data-action="sell">卖出</div>
        <div class="menu-item" data-action="cancel">取消</div> 
    `;

    // 添加点击事件
    menu.querySelectorAll('.menu-item').forEach(item => {
        item.addEventListener('click', () => {
            const action = item.dataset.action;
            if (action === 'cancel') {
                menu.remove(); // 点击取消后立即移除菜单
            } else { // 如果点击的不是取消，则设置交易信息
                document.getElementById('tradeCode').value = code;
                document.getElementById('tradeDirection').value = action; // 设置交易方向
                document.getElementById('tradeAmount').value = 100; // 设置默认交易数量为100
                menu.remove(); // 关闭菜单
                // 手动触发实时价格更新
                const display = document.getElementById('priceDisplay');
                getCurrentPrice(code).then(price => {
                    updatePriceDisplay(display, price);
                });
            }
        });
    });

    // 将菜单添加到页面
    document.body.appendChild(menu);

    // 设置菜单位置（根据鼠标位置或其他逻辑）
    const rect = e.target.getBoundingClientRect();
    menu.style.top = `${rect.top + window.scrollY}px`; // 添加滚动位置偏移
    menu.style.left = `${rect.right + window.scrollX-80}px`; // 使菜单靠右显示

    // 添加右键点击事件来取消菜单
    document.addEventListener('contextmenu', (e) => {
        if (document.contains(menu)) { // 检查菜单是否还在页面中
            menu.remove(); // 如果右键点击，则关闭菜单
        }
    });
}
