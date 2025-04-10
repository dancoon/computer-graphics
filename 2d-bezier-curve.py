import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from math import factorial

# Control points for a cubic Bézier curve (same as in the original code)
ctrl_points = np.array([
    [-4.0, 0.0, 0.0],  # P0
    [-2.0, 4.0, 0.0],  # P1
    [2.0, 4.0, 0.0],   # P2
    [4.0, 0.0, 0.0]    # P3
])

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

# Create figure and axis
fig, ax = plt.subplots(figsize=(10, 10))

# Draw coordinate axes with labels
def draw_axes():
    # X-axis
    ax.arrow(x=-5, y=0, dx=10, dy=0, head_width=0.2, head_length=0.2, 
             fc='r', ec='r', length_includes_head=True)
    
    # Y-axis
    ax.arrow(x=0, y=-5, dx=0, dy=10, head_width=0.2, head_length=0.2, 
             fc='g', ec='g', length_includes_head=True)
    
    # Axis labels
    ax.text(5.2, 0, 'X', color='r', fontsize=12)
    ax.text(0, 5.2, 'Y', color='g', fontsize=12)
    ax.text(0.2, 0.2, 'O', color='black', fontsize=12)
    
    # Draw tick marks
    for i in range(-4, 5):
        if i == 0:
            continue  # Skip origin
        # X-axis ticks
        ax.plot([i, i], [-0.1, 0.1], color='gray')
        # Y-axis ticks
        ax.plot([-0.1, 0.1], [i, i], color='gray')

# Draw the Bézier curve
def draw_bezier_curve():
    # Draw axes
    draw_axes()
    
    # Draw control points
    ax.scatter(ctrl_points[:, 0], ctrl_points[:, 1], color='blue', s=50)
    
    # Draw control polygon
    ax.plot(ctrl_points[:, 0], ctrl_points[:, 1], 'gray', linestyle='-')
    
    # Draw the Bézier curve
    t_values = np.linspace(0, 1, 100)
    curve_points = np.array([calculate_bezier_point(t, ctrl_points) for t in t_values])
    
    ax.plot(curve_points[:, 0], curve_points[:, 1], 'r-', linewidth=2)

# Main function
if __name__ == "__main__":
    # Set equal aspect ratio
    ax.set_aspect('equal')
    
    # Set axis limits
    ax.set_xlim(-5.5, 5.5)
    ax.set_ylim(-5.5, 5.5)
    
    # Set background color
    ax.set_facecolor('white')
    
    # Draw the Bézier curve
    draw_bezier_curve()
    
    # Hide the normal axes
    ax.set_xticks([])
    ax.set_yticks([])
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.spines['bottom'].set_visible(False)
    ax.spines['left'].set_visible(False)
    
    plt.title('2D Bézier Curve with Axes')
    plt.tight_layout()
    plt.show()
