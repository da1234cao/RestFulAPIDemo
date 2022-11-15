
## 环境搭建

ubuntu自带的nodejs版本是v10.19.0。而，[Vite](https://cn.vitejs.dev/guide/#scaffolding-your-first-vite-project)需要的nodejs版本是14.18+，16+。所以我们得切换下版本。参考[如何快速切换和管理node版本](https://juejin.cn/post/7132680379898527757),选择[nvm](https://github.com/nvm-sh/nvm)进行版本管理。

```shell
wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.2/install.sh | bash
nvm -v
nvm install 18
nvm use 18
node -v   
v18.12.1
```

顺手再切换下源吧。(因为走proxy的时候，DNS解析registry.npmjs.org失败了)

```shell
npm config get registry # 查看源
npm config set registry https://registry.npm.taobao.org # 设置仓库
```

创建下项目。

```shell
npm init vue@latest                        
Need to install the following packages:
  create-vue@3.3.4
Ok to proceed? (y) y

Vue.js - The Progressive JavaScript Framework

✔ Project name: … message_board
✔ Add TypeScript? … No / Yes
✔ Add JSX Support? … No / Yes
✔ Add Vue Router for Single Page Application development? … No / Yes
✔ Add Pinia for state management? … No / Yes
✔ Add Vitest for Unit Testing? … No / Yes
✔ Add Cypress for both Unit and End-to-End testing? … No / Yes
✔ Add ESLint for code quality? … No / Yes

Scaffolding project in /home/dacao/work/self/RestFulAPIDemo/fornt_page/message_board...

Done. Now run:

  cd message_board
  npm install
  npm run dev

npm notice 
npm notice New major version of npm available! 8.19.2 -> 9.1.1
npm notice Changelog: https://github.com/npm/cli/releases/tag/v9.1.1
npm notice Run npm install -g npm@9.1.1 to update!
npm notice 
```