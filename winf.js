var windowInfo = require("./");

const list = windowInfo.listWindows();
console.log(
  list.map((x) => ({
    title: windowInfo.windowTitle(x),
    fullScreen: windowInfo.isWindowFullScreen(x),
    rect: windowInfo.windowRect(x),
  }))
);