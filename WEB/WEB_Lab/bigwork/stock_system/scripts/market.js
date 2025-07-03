// scripts/market.js (通用版本)
class Market {
    constructor(config) {
        this.priceHistory = new Map(); // 存储股票代码对应的历史价格
        // 配置参数
        this.tableId = config.tableId;       // 行情表格ID
        this.tabsSelector = config.tabsSelector; // 标签页选择器
        this.countdownId = config.countdownId; // 倒计时元素ID
        this.onStockClick = config.onStockClick || null; // 点击回调

        // 初始化
        this.initMarket();
        this.startAutoRefresh();
    }
    async fetchSingleStock(code) {
        const response = await fetch(`http://localhost:12345/getStockPrice?code=${code}`);
        return response.json();
    }
    initMarket() {
        // 标签页切换
        document.querySelectorAll(this.tabsSelector).forEach(tab => {
            tab.addEventListener('click', () => this.switchMarket(tab.dataset.market));
        });

        // 股票点击事件
        if(this.onStockClick) {
            document.querySelector(`#${this.tableId} tbody`).addEventListener(
                'click',
                e => this.handleStockClick(e)
            );
        }

        // 默认加载沪市
        this.refreshMarket('sh');
    }

    // 更新市场信息
    async refreshMarket(marketType) {
        try {
            const data = await this.fetchMarketData();
            // 处理历史价格
            const processedData = data.map(stock => {
                const code = stock.Code || stock.code;
                const currentPrice = parseFloat(stock.Price || stock.price);
                // 获取上次价格（若无则使用当前价格，涨跌幅为0）
                const prevPrice = this.priceHistory.get(code) || currentPrice; 
                return {
                    ...stock,
                    Price: currentPrice,
                    PrevPrice: prevPrice
                };
            });
            // 更新缓存
            processedData.forEach(stock => {
                this.priceHistory.set(stock.Code || stock.code, stock.Price);
            });
            const filtered = this.filterData(processedData, marketType);
            this.renderTable(filtered);
        } catch (error) {
            console.error('行情更新失败:', error);
        }
    }
    async fetchMarketData() {
        const response = await fetch('http://localhost:12345/getMarketPrice');
        return response.json();
    }

    filterData(data, marketType) {
        return data.filter(stock => {
            const code = stock.Code;
            return (
                (marketType === 'sh' && code.startsWith('6')) ||
                (marketType === 'sz' && code.startsWith('0')) ||
                (marketType === 'cy' && code.startsWith('3'))
            );
        });
    }

    renderTable(data) {
        const tbody = document.querySelector(`#${this.tableId} tbody`);
        tbody.innerHTML = data.map(stock => this.generateRowHTML(stock)).join('');
    }

    generateRowHTML(stock) {
        // 确保字段存在且转换为数字
        const price = parseFloat(stock.Price || stock.price);
        const prevPrice = parseFloat(stock.PrevPrice || stock.prevPrice);
    
        return `
            <tr data-code="${stock.Code || stock.code}">
                <td class="stock-code">${stock.Code || stock.code}</td>
                <td>${stock.Name || stock.name}</td>
                <td>${price.toFixed(2)}</td>
                <td class="${this.getChangeClass({ Price: price, PrevPrice: prevPrice })}">
                    ${this.calculateChangePercent({ Price: price, PrevPrice: prevPrice })}%
                </td>
                <td>${this.calculateChangeAmount({ Price: price, PrevPrice: prevPrice })}</td>
            </tr>
        `;
    }
    //如果个股被点击，可以进行相关操作
    handleStockClick(e) {
        const row = e.target.closest('tr');
        if (row) {
            const code = row.dataset.code;
            this.onStockClick(code, row);
    
            // 显示自定义弹出菜单
            this.showActionMenu(code, e);
        }
    }
    
   // 显示操作菜单
   showActionMenu(code, e) {
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


    // 工具方法
    getChangeClass(stock) {
        return stock.Price > stock.PrevPrice ? 'up' : 'down';
    }

    // 直接使用处理后的数据
    calculateChangePercent(stock) {
        const prev = stock.PrevPrice || stock.Price; // 兜底逻辑
        return (((stock.Price - prev) / prev * 100) || 0).toFixed(2);
    }

    calculateChangeAmount(stock) {
        return (stock.Price - (stock.PrevPrice || stock.Price)).toFixed(2);
    }
    switchMarket(marketType) {
        document.querySelectorAll(this.tabsSelector).forEach(t => t.classList.remove('active'));
        event.target.classList.add('active');
        this.refreshMarket(marketType);
    }

    startAutoRefresh(interval = 5000) {
        let countdown = 5;
        this.timer = setInterval(() => {
            countdown = countdown > 0 ? countdown - 1 : 5;
            if(this.countdownId) {
                document.getElementById(this.countdownId).textContent = `${countdown}s`;
            }
            if(countdown === 0) {
                const activeTab = document.querySelector(`${this.tabsSelector}.active`);
                this.refreshMarket(activeTab.dataset.market);
            }
        }, 1000);
    }

    destroy() {
        clearInterval(this.timer);
    }
    
}
// 个股点击事件处理（补充走势图加载）
document.addEventListener('DOMContentLoaded', () => {
    const userMarket = new Market({
        // ... 原有配置 ...
        onStockClick: (code, row) => {
            userMarket.fetchSingleStock(code).then(prices => {
                //loadStockChart(code);
                createBeautifulChart(code,prices);
                highlightSelectedRow(row);
            });
        }
    });
});