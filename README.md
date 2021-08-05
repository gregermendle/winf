# Window Info [WIP]

Native module for OSX and Windows to get information about application windows running on a system.

## API
The API is currently incomplete. A planned API is shown below.

#### `isFullScreen(title: string): boolean`
[OSX Only] Returns a boolean determining if a window with the defined title is full screen.

## Planned API
#### `findWindowByTitle(title: string): Window | null`
Returns a window handle for the first window that matches the provided title, otherwise `null`.

#### `listWindows(): Array<Window>`
Returns a list of all windows open on the system.

#### `windowRect(window: Window): Rect`
Returns the position and dimensions of a given window.

#### `windowTitle(window: Window): string | null`
Returns the title of a given window, otherwise `null`.

#### `isWindowFullScreen(window: Window): boolean`
Returns a boolean determining if a window is full screen.

#### `windowPid(window: Window): number | null`
Returns the window's owning processes pid
