# Githup的使用
第一个命令初始化本地仓库
git init
进入 .git
创建.md文件.
然后git status.查看当前代码仓库的状态．
git add Github.md 添加到仓库
git status
git rm --cached Github 删除追踪的状态文件 git 相当于二级命令.
删除后变成未追踪的第一次显示的样子．
git commit　就是提交当前追踪到的所有修改. 
git commit -m "init Github.md"　就是直接用命令添加到信息 -m就是修改那个文件
git remote add origin https://github.com/lys-studys/GitHub.git 远端备份
git push -u origin master 上传信息
