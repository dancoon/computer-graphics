import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.widgets import Slider
from math import factorial

# Control points for a cubic Bézier curve in 3D (same as in the original code)
ctrl_points = np.array([
    [-4.0, 0.0, -2.0],  # P0
    [-2.0, 4.0, 2.0],   # P1
    [2.0, 4.0, 0.0],    # P2
    [4.0, 0.0, 3.0]     # P3
])

# Initial rotation angles
x_rot = 20.0
y_rot = 30.0
z_rot = 0.0

# Function to calculate combination nCr
def combination(n, r):
    return factorial(n) // (factorial(r) * factorial(n - r))

# Calculate a point on Bézier curve using Bernstein basis
def calculate_bezier_point(t, control_points):
    num_control_points = len(control_points)
    point = np.zeros(3)
    
    for i in range(num_control_points):
        bernstein = combination(num_control_points - 1, i) * \
                   (t ** i) * \
                   ((1 - t) ** (num_control_points - 1 - i))
        
        point += control_points[i] * bernstein
    
    return point

# Create the figure and 3D axis
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Create sliders for rotation
ax_x_slider = plt.axes([0.25, 0.02, 0.65, 0.03])
ax_y_slider = plt.axes([0.25, 0.06, 0.65, 0.03])
ax_z_slider = plt.axes([0.25, 0.10, 0.65, 0.03])

x_slider = Slider(ax_x_slider, 'X Rotation', 0, 360, valinit=x_rot)
y_slider = Slider(ax_y_slider, 'Y Rotation', 0, 360, valinit=y_rot)
z_slider = Slider(ax_z_slider, 'Z Rotation', 0, 360, valinit=z_rot)

# Draw the 3D Bézier curve with coordinate axes
def draw_bezier_curve(x_rot, y_rot, z_rot):
    ax.clear()
    
    # Set the limits
    ax.set_xlim([-5, 5])
    ax.set_ylim([-5, 5])
    ax.set_zlim([-5, 5])
    
    # Set the labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    
    # Set the view angle based on rotations
    ax.view_init(elev=x_rot, azim=y_rot)  # Note: Matplotlib doesn't support z-axis rotation easily
    
    # Draw coordinate axes
    # X-axis (red)
    ax.plot([-5, 5], [0, 0], [0, 0], 'r-', linewidth=2)
    # Y-axis (green)
    ax.plot([0, 0], [-5, 5], [0, 0], 'g-', linewidth=2)
    # Z-axis (blue)
    ax.plot([0, 0], [0, 0], [-5, 5], 'b-', linewidth=2)
    
    # Add axis labels
    ax.text(5.3, 0, 0, "X", color='red')
    ax.text(0, 5.3, 0, "Y", color='green')
    ax.text(0, 0, 5.3, "Z", color='blue')
    ax.text(0.2, 0.2, 0.2, "O", color='black')
    
    # Draw grid on XZ plane
    for i in range(-5, 6):
        if i == 0:
            continue  # Skip the axes
        # Lines parallel to X-axis
        ax.plot([-5, 5], [0, 0], [i, i], 'gray', linewidth=0.5, alpha=0.5)
        # Lines parallel to Z-axis
        ax.plot([i, i], [0, 0], [-5, 5], 'gray', linewidth=0.5, alpha=0.5)
    
    # Draw control points
    ax.scatter(ctrl_points[:, 0], ctrl_points[:, 1], ctrl_points[:, 2], color='blue', s=50)
    
    # Draw control polygon
    ax.plot(ctrl_points[:, 0], ctrl_points[:, 1], ctrl_points[:, 2], 'gray', linestyle='-')
    
    # Draw vertical lines from control points to XZ plane
    for i in range(4):
        x, y, z = ctrl_points[i]
        ax.plot([x, x], [y, 0], [z, z], 'gray', linestyle='--', alpha=0.7)
    
    # Draw the Bézier curve
    t_values = np.linspace(0, 1, 100)
    curve_points = np.array([calculate_bezier_point(t, ctrl_points) for t in t_values])
    
    ax.plot(curve_points[:, 0], curve_points[:, 1], curve_points[:, 2], 'r-', linewidth=2.5)
    
    # Add curve label
    mid_point = calculate_bezier_point(0.5, ctrl_points)
    ax.text(mid_point[0], mid_point[1] + 0.5, mid_point[2], "Bézier Curve", color='black')
    
    # Display rotation information
    title = f"Rotation: X={x_rot:.1f}, Y={y_rot:.1f}, Z={z_rot:.1f}"
    ax.set_title(title)
    
    # Make the background white
    ax.set_facecolor('white')
    
    fig.canvas.draw_idle()

# Update function for sliders
def update(val):
    x_rot = x_slider.val
    y_rot = y_slider.val
    z_rot = z_slider.val
    draw_bezier_curve(x_rot, y_rot, z_rot)

# Connect the sliders to the update function
x_slider.on_changed(update)
y_slider.on_changed(update)
z_slider.on_changed(update)

# Initial draw
draw_bezier_curve(x_rot, y_rot, z_rot)

# Add a title
plt.subplots_adjust(bottom=0.15)
fig.suptitle('3D Bézier Curve with Coordinate Axes', fontsize=16)

# Display the plot
plt.show()
