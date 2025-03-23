# tpiu_itm.ps1 (修正版)
$ocdPort = 4444  # OpenOCD 控制端口
$itmPort = 3444   # ITM 输出端口

# 步骤1：发送配置命令到 OpenOCD
$ocdCommands = @"
tpiu config internal :$itmPort uart off 168000000 2000000
itm port 0 on
"@

# 使用 TCP 客户端直接发送命令（非 Telnet 交互模式）
$tcpClient = New-Object System.Net.Sockets.TcpClient('localhost', $ocdPort)
$stream = $tcpClient.GetStream()
$writer = New-Object System.IO.StreamWriter($stream)

# 发送命令并确保换行符
$writer.WriteLine($ocdCommands)
$writer.Flush()

# 步骤2：启动独立 ITM 数据监听（需提前安装 Telnet 客户端）
Start-Sleep -Milliseconds 500  # 等待配置生效
Start-Process cmd.exe -ArgumentList "/k telnet localhost $itmPort"
