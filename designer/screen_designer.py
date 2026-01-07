import tkinter as tk
from tkinter import filedialog, messagebox
import struct

class PixelDesigner:
    def __init__(self, root):
        self.root = root
        self.root.title("Pixel Designer - 128x64 Grid")

        # Grid dimensions
        self.width = 128
        self.height = 64

        # Pixel size for display (adjust for visibility)
        self.pixel_size = 10

        # Initialize pixel grid (False = off/black, True = on/white)
        self.pixels = [[False for _ in range(self.width)] for _ in range(self.height)]

        # UI state
        self.dragging = False
        self.drag_value = False  # Whether to turn pixels on or off during drag
        self.last_x = 0
        self.last_y = 0

        # Create main frame
        self.main_frame = tk.Frame(root)
        self.main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)

        # Create canvas for pixel grid
        self.canvas = tk.Canvas(
            self.main_frame,
            width=self.width * self.pixel_size,
            height=self.height * self.pixel_size,
            bg='black',
            cursor='crosshair'
        )
        self.canvas.pack(side=tk.TOP, pady=(0, 10))

        # Create control frame
        self.control_frame = tk.Frame(self.main_frame)
        self.control_frame.pack(side=tk.TOP, fill=tk.X)

        # Clear button
        self.clear_button = tk.Button(
            self.control_frame,
            text="Clear All",
            command=self.clear_all,
            bg='#ff6b6b',
            fg='white',
            padx=20,
            pady=5
        )
        self.clear_button.pack(side=tk.LEFT, padx=(0, 10))

        # Invert button
        self.invert_button = tk.Button(
            self.control_frame,
            text="Invert",
            command=self.invert_all,
            bg='#4ecdc4',
            fg='white',
            padx=20,
            pady=5
        )
        self.invert_button.pack(side=tk.LEFT, padx=(0, 10))

        # Save button
        self.save_button = tk.Button(
            self.control_frame,
            text="Save Binary",
            command=self.save_binary,
            bg='#45b7d1',
            fg='white',
            padx=20,
            pady=5
        )
        self.save_button.pack(side=tk.LEFT)

        # Status label
        self.status_label = tk.Label(
            self.control_frame,
            text="Ready",
            anchor='w'
        )
        self.status_label.pack(side=tk.RIGHT, padx=(10, 0))

        # Bind mouse events
        self.canvas.bind('<Button-1>', self.on_mouse_down)
        self.canvas.bind('<B1-Motion>', self.on_mouse_drag)
        self.canvas.bind('<ButtonRelease-1>', self.on_mouse_up)

        # Draw initial grid
        self.draw_grid()

        # Update status
        self.update_status()

    def canvas_to_grid(self, x, y):
        """Convert canvas coordinates to grid coordinates"""
        grid_x = x // self.pixel_size
        grid_y = y // self.pixel_size

        # Clamp to grid bounds
        grid_x = max(0, min(grid_x, self.width - 1))
        grid_y = max(0, min(grid_y, self.height - 1))

        return grid_x, grid_y

    def toggle_pixel(self, x, y):
        """Toggle a single pixel"""
        if 0 <= x < self.width and 0 <= y < self.height:
            self.pixels[y][x] = not self.pixels[y][x]
            self.draw_pixel(x, y)

    def set_pixel(self, x, y, value):
        """Set a pixel to a specific value"""
        if 0 <= x < self.width and 0 <= y < self.height:
            if self.pixels[y][x] != value:
                self.pixels[y][x] = value
                self.draw_pixel(x, y)

    def draw_pixel(self, x, y):
        """Draw a single pixel on the canvas"""
        color = 'white' if self.pixels[y][x] else 'black'
        x1 = x * self.pixel_size
        y1 = y * self.pixel_size
        x2 = x1 + self.pixel_size
        y2 = y1 + self.pixel_size

        # Clear the pixel area first
        self.canvas.create_rectangle(x1, y1, x2, y2, fill=color, outline='')

    def draw_grid(self):
        """Draw the entire grid"""
        self.canvas.delete('all')

        for y in range(self.height):
            for x in range(self.width):
                self.draw_pixel(x, y)

    def on_mouse_down(self, event):
        """Handle mouse button press"""
        self.dragging = True
        x, y = self.canvas_to_grid(event.x, event.y)

        # Toggle the clicked pixel
        old_value = self.pixels[y][x]
        self.drag_value = not old_value
        self.set_pixel(x, y, self.drag_value)

        self.last_x, self.last_y = x, y
        self.update_status()

    def on_mouse_drag(self, event):
        """Handle mouse drag"""
        if not self.dragging:
            return

        x, y = self.canvas_to_grid(event.x, event.y)

        # Only update if we've moved to a different pixel
        if x != self.last_x or y != self.last_y:
            # Fill pixels in a line from last position to current position
            self.fill_line(self.last_x, self.last_y, x, y, self.drag_value)
            self.last_x, self.last_y = x, y

    def on_mouse_up(self, event):
        """Handle mouse button release"""
        self.dragging = False

    def fill_line(self, x1, y1, x2, y2, value):
        """Fill pixels in a line from (x1,y1) to (x2,y2)"""
        # Bresenham's line algorithm for filling pixels during drag
        dx = abs(x2 - x1)
        dy = abs(y2 - y1)
        sx = 1 if x1 < x2 else -1
        sy = 1 if y1 < y2 else -1
        err = dx - dy

        while True:
            self.set_pixel(x1, y1, value)

            if x1 == x2 and y1 == y2:
                break

            e2 = 2 * err
            if e2 > -dy:
                err -= dy
                x1 += sx
            if e2 < dx:
                err += dx
                y1 += sy

    def clear_all(self):
        """Clear all pixels"""
        for y in range(self.height):
            for x in range(self.width):
                self.pixels[y][x] = False
        self.draw_grid()
        self.update_status()

    def invert_all(self):
        """Invert all pixels"""
        for y in range(self.height):
            for x in range(self.width):
                self.pixels[y][x] = not self.pixels[y][x]
        self.draw_grid()
        self.update_status()

    def save_binary(self):
        """Save the pixel grid as a binary file"""
        filename = filedialog.asksaveasfilename(
            defaultextension=".bin",
            filetypes=[("Binary files", "*.bin"), ("All files", "*.*")],
            title="Save Pixel Data"
        )

        if not filename:
            return

        try:
            with open(filename, 'wb') as f:
                # Write pixels row by row, left to right, top to bottom
                for y in range(self.height):
                    for x in range(self.width):
                        # Write as single bits (0 or 1)
                        bit = 1 if self.pixels[y][x] else 0
                        f.write(struct.pack('B', bit))

            messagebox.showinfo("Success", f"Pixel data saved to {filename}")
            self.status_label.config(text=f"Saved: {filename}")

        except Exception as e:
            messagebox.showerror("Error", f"Failed to save file: {str(e)}")

    def update_status(self):
        """Update the status label with pixel count"""
        on_pixels = sum(sum(row) for row in self.pixels)
        total_pixels = self.width * self.height
        percentage = (on_pixels / total_pixels) * 100
        self.status_label.config(text=".1f")

def main():
    root = tk.Tk()
    app = PixelDesigner(root)
    root.mainloop()

if __name__ == "__main__":
    main()
