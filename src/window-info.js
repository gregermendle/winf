var windowInfo = require('../build/Release/window-info');

const title = "League of Legends (TM) Client";

setInterval(() => {
  console.log(`${title} is fullscreen? `, windowInfo.isFullScreen(title));
}, 2000);
