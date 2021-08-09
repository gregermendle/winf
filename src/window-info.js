var windowInfo = require("../build/Release/window-info");

// const title = "League of Legends (TM) Client";

const list = windowInfo.listWindows();
console.log(list);
console.log(list.map(windowInfo.windowRect));

// setInterval(() => {
//   console.log(`${title} is fullscreen? `, windowInfo.isFullScreen(title));
// }, 2000);
