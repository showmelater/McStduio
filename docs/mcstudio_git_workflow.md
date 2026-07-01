# McStudio Git 分支与二次开发流程说明

> 项目目录：`/home/aa/Desktop/McStduio`  
> Fork 仓库：`https://github.com/showmelater/McStduio`  
> 官方上游仓库：`https://github.com/FreeCAD/FreeCAD`

---

## 1. 当前仓库关系

本地仓库已经配置为：

```text
origin    https://github.com/showmelater/McStduio.git    fetch/push
upstream  https://github.com/FreeCAD/FreeCAD.git         fetch
upstream  DISABLED                                      push
```

含义：

| remote | 用途 | 是否推送 |
|---|---|---|
| `origin` | 你自己的 fork 仓库 | 可以 push |
| `upstream` | FreeCAD 官方仓库 | 只 fetch，不 push |

`upstream` 的 push 地址被设置成 `DISABLED`，这是为了防止误推送到官方 FreeCAD 仓库。

查看 remote：

```bash
cd /home/aa/Desktop/McStduio
git remote -v
```

---

## 2. 当前分支策略

推荐分支结构：

```text
main
└── mcstudio/dev
    ├── feature/mcstudio-workbench
    ├── feature/mcstudio-menu
    ├── feature/mcstudio-plane
    ├── feature/mcstudio-taskpanel
    ├── feature/mcstudio-branding
    └── fix/mcstudio-build-error
```

分支含义：

| 分支 | 作用 |
|---|---|
| `main` | 尽量保持和 fork 默认分支一致，方便同步官方 FreeCAD |
| `mcstudio/dev` | McStudio 二次开发主分支，以后主要功能都合并到这里 |
| `feature/*` | 每个具体功能单独开分支开发 |
| `fix/*` | 修复问题用的分支 |
| `docs/*` | 文档整理分支 |

当前已经创建并切换到：

```text
mcstudio/dev
```

查看当前分支：

```bash
git branch --show-current
```

查看所有分支：

```bash
git branch -vv
```

---

## 3. 第一次推送开发分支

如果 `mcstudio/dev` 还没有推送到 GitHub，需要执行：

```bash
cd /home/aa/Desktop/McStduio
git push -u origin mcstudio/dev
```

执行后，GitHub 上会出现：

```text
mcstudio/dev
```

以后在 `mcstudio/dev` 分支上执行：

```bash
git push
```

就会自动推送到：

```text
origin/mcstudio/dev
```

---

## 4. 日常开发流程

不要直接在 `main` 上开发，也尽量不要直接在 `mcstudio/dev` 上写大功能。

推荐流程：

```bash
cd /home/aa/Desktop/McStduio

# 回到开发主分支
git checkout mcstudio/dev

# 确保本地是最新的
git pull

# 从 dev 新建功能分支
git checkout -b feature/mcstudio-workbench
```

开发完成后：

```bash
git status

git add src/Mod/McStudio src/Mod/CMakeLists.txt

git commit -m "feat: add McStudio workbench"

git push -u origin feature/mcstudio-workbench
```

然后可以在 GitHub 上创建 Pull Request，把：

```text
feature/mcstudio-workbench
```

合并到：

```text
mcstudio/dev
```

---

## 5. 推荐分支命名

```text
mcstudio/dev                 二次开发主分支

feature/mcstudio-workbench   新增 McStudio 工作台
feature/mcstudio-menu        新增 McStudio 菜单/工具栏
feature/mcstudio-plane       自定义参考平面
feature/mcstudio-coordinate  自定义坐标系
feature/mcstudio-taskpanel   自定义任务面板
feature/mcstudio-dock        自定义 Dock 面板
feature/mcstudio-branding    品牌、图标、启动页
feature/mcstudio-build       编译、打包、安装

fix/mcstudio-build-error     修复构建问题
fix/mcstudio-gui-crash       修复 GUI 崩溃

docs/gui-architecture        GUI 架构文档
docs/git-workflow            Git 流程文档
```

---

## 6. 和官方 FreeCAD 同步

以后官方 FreeCAD 有更新时，不要直接在 `mcstudio/dev` 上拉官方代码，建议先更新 `main`。

### 6.1 更新 main 到官方最新

```bash
cd /home/aa/Desktop/McStduio

# 切回 main
git checkout main

# 拉取官方仓库信息
git fetch upstream

# 把官方 main 合并到本地 main
git merge upstream/main

# 推送到自己的 fork
git push origin main
```

### 6.2 把官方更新合并进 McStudio 开发分支

```bash
# 切回 McStudio 开发分支
git checkout mcstudio/dev

# 把 main 的更新合并进来
git merge main

# 推送开发分支
git push origin mcstudio/dev
```

如果出现冲突，解决冲突后执行：

```bash
git status

git add <解决冲突后的文件>

git commit

git push origin mcstudio/dev
```

---

## 7. 二次开发建议从哪个分支开始

第一个功能建议新建：

```text
feature/mcstudio-workbench
```

命令：

```bash
cd /home/aa/Desktop/McStduio

git checkout mcstudio/dev

git checkout -b feature/mcstudio-workbench
```

第一阶段建议新增：

```text
src/Mod/McStudio/
src/Mod/McStudio/Init.py
src/Mod/McStudio/InitGui.py
src/Mod/McStudio/CMakeLists.txt
```

并修改：

```text
src/Mod/CMakeLists.txt
```

加入：

```cmake
add_subdirectory(McStudio)
```

---

## 8. McStudio 二次开发推荐路线

推荐顺序：

```text
第一阶段：新增 McStudio 工作台
第二阶段：新增 McStudio 菜单和工具栏
第三阶段：新增 McStudio 命令系统
第四阶段：新增自定义参考平面对象
第五阶段：新增自定义任务面板
第六阶段：新增 Dock 面板和项目树
第七阶段：品牌化，改图标、启动页、名称
第八阶段：打包发布
```

不要一开始做这些：

```text
不要全局替换 FreeCAD 为 McStudio
不要直接修改 Document 保存格式
不要直接删掉 FreeCAD 原有 Workbench
不要一开始改 View3DInventorViewer
不要一开始大改 MainWindow
不要先做安装包
```

正确方式是先做独立模块：

```text
src/Mod/McStudio
```

把自己的功能尽量封装在模块里，减少和 FreeCAD 核心代码的冲突。

---

## 9. 当前未跟踪文件处理建议

当前本地可能有这些未跟踪文件：

```text
demo/
docs/
区别.md
开发手册.md
```

建议：

| 文件/目录 | 建议 |
|---|---|
| `docs/` | 可以加入 git，作为项目文档 |
| `开发手册.md` | 可以加入 git |
| `区别.md` | 可以加入 git |
| `demo/` | 如果是测试数据，先确认内容，再决定是否加入 |

提交文档示例：

```bash
git add docs/ 开发手册.md 区别.md

git commit -m "docs: add McStudio development documents"

git push
```

如果 `demo/` 不想加入 git，可以先在 `.gitignore` 中忽略，或暂时不处理。

---

## 10. 常用 Git 命令

### 查看状态

```bash
git status --short --branch
```

### 查看分支

```bash
git branch -vv
```

### 查看 remote

```bash
git remote -v
```

### 创建功能分支

```bash
git checkout mcstudio/dev
git checkout -b feature/功能名
```

### 提交修改

```bash
git status
git add <文件或目录>
git commit -m "feat: 描述你的功能"
```

### 推送分支

```bash
git push -u origin 当前分支名
```

### 拉取官方更新

```bash
git fetch upstream
```

### 查看最近提交

```bash
git log --oneline --decorate -10
```

### 查看本地和远程差异

```bash
git status
git diff
git diff --stat
```

---

## 11. 推荐提交信息格式

建议使用简单清晰的提交信息：

```text
feat: add McStudio workbench
feat: add McStudio reference plane command
fix: resolve McStudio build error
docs: add Git workflow document
refactor: move McStudio commands into separate file
style: update McStudio icons
build: add McStudio CMake module
```

常用前缀：

| 前缀 | 用途 |
|---|---|
| `feat:` | 新功能 |
| `fix:` | 修复 bug |
| `docs:` | 文档 |
| `build:` | 构建系统 |
| `refactor:` | 重构 |
| `style:` | 格式、图标、样式 |
| `test:` | 测试 |

---

## 12. 最终建议

以后二次开发遵循这条规则：

```text
main：尽量保持干净，用来跟官方 FreeCAD 同步
mcstudio/dev：McStudio 二次开发主分支
feature/*：每个功能单独开发
```

第一件事：

```bash
git push -u origin mcstudio/dev
```

第二件事：

```bash
git checkout mcstudio/dev
git checkout -b feature/mcstudio-workbench
```

然后开始新增：

```text
src/Mod/McStudio
```
