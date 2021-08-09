var windowInfo = require("../build/Release/window-info");

const title = "League of Legends (TM) Client";

const list = windowInfo.listWindows();
console.log(list.map(windowInfo.windowTitle));

setInterval(() => {
  console.log(`${title} is fullscreen? `, windowInfo.isFullScreen(title));
}, 2000);
