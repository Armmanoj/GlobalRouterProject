import matplotlib.pyplot as plt
import numpy as np
import sys

def read_net_file(file_path):
    nets = {}
    with open(file_path, 'r') as file:
        lines = file.read().splitlines()
        current_net = None
        for line in lines:
            line = line.strip()
            if line and not (line.startswith('(') or line.startswith('[') or line.startswith(')')):
                current_net = line
                nets[current_net] = []
            elif line.startswith('['):
                # Remove the enclosing parentheses
                group = line
                if group:
                    # Extract only the first point of each group
                    first_point = tuple(map(int, group[group.find('(')+1:group.find(')')].split(',')))
                    nets[current_net].append(first_point)
    return nets



def read_cap_file(file_path):
    with open(file_path, 'r') as file:
        lines = file.read().splitlines()
        l, x, y = map(int, lines[0].split())
        return l, x, y

def read_pr_output_file(file_path):
    routes = {}
    with open(file_path, 'r') as file:
        lines = file.read().splitlines()
        current_net = None
        flag = 0
        for line in lines:
            line = line.strip()
            if line and not (line.startswith('(') or flag or line.startswith(')')):
                flag = 1
                current_net = line
                routes[current_net] = []
            elif line and not (line.startswith('(') or line.startswith(')')):
                result = tuple(map(int, line.split()))
                routes[current_net].append(result)
            elif line.startswith(')'):
                flag = 0
    return routes

def plot_nets_and_routes(nets, routes, l, x, y):
    layer_colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k']
    markers = ['o', '^', 's', 'p', '*', 'X', 'D']
    
    fig, ax = plt.subplots()
    ax.set_xlim(-1, x)
    ax.set_ylim(-1, y)
    
    net_ids = list(nets.keys())
    for i, net_id in enumerate(net_ids):
        net_points = nets[net_id]
        net_routes = routes.get(net_id, [])
        
        marker = markers[i % len(markers)]
        
        # Plot points
        for point in net_points:
            l, x, y = point
            color = layer_colors[l % len(layer_colors)]
            ax.plot(x, y, color=color, marker=marker, linestyle='None', label=f'{net_id} Layer {l}' if (x == net_points[0][1] and y == net_points[0][2]) else "")
        
        # Plot routes
        for route in net_routes:
            x1, y1, l1, x2, y2, l2 = route
            if l1 == l2:
                color = layer_colors[l1 % len(layer_colors)]
                linewidth = 2.5  # Adjust line thickness as needed
                alpha = 0.5      # Adjust transparency (0 is fully transparent, 1 is fully opaque)
                # Plot line for current layer
                ax.plot([x1, x2], [y1, y2], color=color, linestyle='-', linewidth=linewidth, alpha=alpha)
    
    handles, labels = ax.get_legend_handles_labels()
    unique_labels = list(dict.fromkeys(labels))
    unique_handles = [handles[labels.index(label)] for label in unique_labels]
    #ax.legend(unique_handles, unique_labels, loc='upper right')

    #plt.grid(True)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Nets and Routes')
    plt.show()

# File paths
cap_file_path = sys.argv[1]
net_file_path = sys.argv[2]
pr_output_file_path = sys.argv[3]

# Read the files
nets = read_net_file(net_file_path)
l, x, y = read_cap_file(cap_file_path)
routes = read_pr_output_file(pr_output_file_path)

# Plot the nets and routes
plot_nets_and_routes(nets, routes, l, x, y)
