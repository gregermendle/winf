# Window Info

Get basic info about windows on OSX and Windows.

## API

Get a list of all windows open on the system.
```tsx
listWindows(): Array<Window>
```

Get the position and dimensions of a window.
```tsx
windowRect(window: Window): Rect
```

Get the title of a window, otherwise `null`.
```tsx
windowTitle(window: Window): string | null
```

Determine if a window is full screen.
```tsx
isWindowFullScreen(window: Window): boolean
```

