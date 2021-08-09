var windowInfo = require("../build/Release/window-info");

const list = windowInfo.listWindows();
console.log(
  list.map((x) => ({
    title: windowInfo.windowTitle(x),
    fullScreen: windowInfo.isWindowFullScreen(x),
    rect: windowInfo.windowRect(x),
  }))
);
