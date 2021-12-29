# SSevGui
shadowsocks-libev的Qt5的GUI封装

为的是在Ubuntu上正常使用SSD的订阅和图形界面。

为了偷懒，大量的代码照搬了“shadowsocks-qt5”项目
还有部分逻辑参照TheCGDF的“SSD”项目
感谢项目组和TheCGDF所做的贡献。

**生成的可执行文件需要ss-local与obfs-local放置在同一目录才能正常开启，目前只支持ssd订阅和simple-obfs插件。**

（因为不知道其他订阅怎么解析，也不知道其他插件怎么用）

----------------------------

2021-12-28 更新

- 集成gsettings命令行的设置，免去更改系统设置的麻烦；
- 托盘菜单有了服务器列表，重复点击当前可以服务器可以关闭SS。

## Ubuntu关闭系统代理的命令行
若由于Bug导致网络问题，可以尝试关闭系统代理看能否恢复。
>gsettings set org.gnome.system.proxy mode 'none'

不支持Gsettings的则需要手动配置系统代理。
