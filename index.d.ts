// window-info.d.ts

interface Rect {
  x: number;
  y: number;
  width: number;
  height: number;
}

type WindowId = number;

declare module "window-info" {
  /**
   * Returns a list of all windows that are open and visible on the system.
   * @returns A list of numeric window ids.
   */
  export function listWindows(): WindowId[];

  /**
   * Returns the position and dimensions of a given window.
   * @param window - The window to get information for.
   * @returns The position and dimensions of the window.
   */
  export function windowRect(window: WindowId): Rect;

  /**
   * Returns the title of a given window, otherwise `null`.
   * @param window - The window to get the title for.
   * @returns The title of the window or `null` if not available.
   */
  export function windowTitle(window: WindowId): string | null;

  /**
   * Returns a boolean determining if a window is full screen.
   * @param window - The window to check for full-screen status.
   * @returns `true` if the window is full screen, otherwise `false`.
   */
  export function isWindowFullScreen(window: WindowId): boolean;
}
