import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import os
import time
import numpy as np

FILE_NAME = "aco_data.txt"

#    0   1   2   3   4   5   6   7   8
STATIC_DISTANCES = np.array([
    [0, 25, 40, 60, 15, 88, 30, 45, 75], # 0
    [15, 0, 18, 55, 32, 21, 62, 10, 50], # 1
    [35, 20, 0, 27, 45, 30, 12, 55, 38], # 2
    [50, 45, 20, 0, 10, 70, 48, 22, 16], # 3
    [12, 35, 42, 10, 0, 50, 25, 30, 28], # 4
    [80, 20, 35, 65, 52, 0, 90, 78, 11], # 5
    [32, 60, 10, 50, 28, 95, 0, 40, 53], # 6
    [40, 12, 60, 25, 35, 80, 42, 0, 18], # 7
    [70, 55, 30, 18, 30, 15, 50, 15, 0]  # 8
])
N_STATIC_CITIES = 9

STATIC_COORDS = [
    (10, 80), (30, 10), (50, 90), (70, 20), (90, 70), 
    (10, 50), (30, 60), (70, 40), (90, 30)
]


def load_data(filename):
    data = []
    current_iter_data = {}
    
    if not os.path.exists(filename):
        return data

    with open(filename, 'r') as f:
        lines = f.readlines()

    for line in lines:
        line = line.strip()
        if not line:
            continue

        parts = line.split(',', 1)
        if len(parts) < 2: continue
        key, value = parts

        if key == "ITERATION":
            if current_iter_data and 'COORDS' in current_iter_data:
                data.append(current_iter_data)
            current_iter_data = {'ITERATION': int(value)}
        
        elif key == "LENGTH":
            current_iter_data['LENGTH'] = float(value)
        
        elif key == "COORDS":
            coords = []
            for pair in value.split('|'):
                if ';' in pair:
                    x, y = map(float, pair.split(';'))
                    coords.append((x, y))
            current_iter_data['COORDS'] = coords
            
        elif key == "ROUTE":
            route_str = value.replace('->', ' ').split()
            current_iter_data['ROUTE'] = [int(r) for r in route_str]

    if current_iter_data and 'COORDS' in current_iter_data:
        data.append(current_iter_data)
    return data

def calculate_distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)

def update_plot(frame, data, scat, line, coords_text, dist_labels, ax):
    if frame >= len(data):
        return
        
    current_data = data[frame]
    coords = current_data['COORDS']
    route = current_data['ROUTE']
    
    ax.set_title(f"Итерация: {current_data['ITERATION']} | Общий путь: {current_data['LENGTH']:.2f}")

    x = [c[0] for c in coords]
    y = [c[1] for c in coords]
    scat.set_offsets(list(zip(x, y)))
    
    route_x = []
    route_y = []
    
    for city_idx in route:
        route_x.append(coords[city_idx][0])
        route_y.append(coords[city_idx][1])
    route_x.append(coords[route[0]][0])
    route_y.append(coords[route[0]][1])
    
    line.set_data(route_x, route_y)
    line.set_color('blue') 

    for txt in coords_text:
        txt.remove()
    coords_text.clear()
    
    for i, (cx, cy) in enumerate(coords):
        txt = ax.annotate(str(i), (cx, cy), xytext=(0, 8), textcoords="offset points", 
                          ha='center', fontsize=9, color='darkblue', fontweight='bold')
        coords_text.append(txt)

    for txt in dist_labels:
        txt.remove()
    dist_labels.clear()

    full_route = route + [route[0]] 
    
    for i in range(len(full_route) - 1):
        idx1 = full_route[i]
        idx2 = full_route[i+1]
        
        p1 = coords[idx1]
        p2 = coords[idx2]
        
        dist = calculate_distance(p1, p2)
        
        mid_x = (p1[0] + p2[0]) / 2
        mid_y = (p1[1] + p2[1]) / 2
        
        lbl = ax.text(mid_x, mid_y, f"{dist:.1f}", 
                      fontsize=8, color='red', ha='center', va='center',
                      bbox=dict(boxstyle="round,pad=0.2", fc="white", ec="none", alpha=0.7))
        dist_labels.append(lbl)

    return scat, line

def draw_static_graph(ax):
    coords = STATIC_COORDS
    
    all_x = [c[0] for c in coords]
    all_y = [c[1] for c in coords]
    margin = 10
    ax.set_xlim(min(all_x) - margin, max(all_x) + margin)
    ax.set_ylim(min(all_y) - margin, max(all_y) + margin)

    ax.set_title("Статический граф городов (N=9) с расстояниями", fontsize=14, fontweight='bold')
    ax.set_xlabel("X-координата")
    ax.set_ylabel("Y-координата")
    ax.set_xticks([])
    ax.set_yticks([])
    ax.grid(False)

    for i in range(N_STATIC_CITIES):
        for j in range(i + 1, N_STATIC_CITIES):
            p1 = coords[i]
            p2 = coords[j]
            ax.plot([p1[0], p2[0]], [p1[1], p2[1]], color='lightgray', linestyle='-', linewidth=3.0, zorder=1) 

    ax.scatter(all_x, all_y, color='black', s=300, zorder=5)

    distance_labels = []
    
    for i in range(N_STATIC_CITIES):
        ax.annotate(str(i), (coords[i][0], coords[i][1]), xytext=(0, 0), textcoords="offset points", 
                          ha='center', va='center', fontsize=16, color='white', fontweight='bold', zorder=6) 
                          
        for j in range(i + 1, N_STATIC_CITIES):
            p1 = coords[i]
            p2 = coords[j]
            dist = STATIC_DISTANCES[i, j]
            
            mid_x = (p1[0] + p2[0]) / 2
            mid_y = (p1[1] + p2[1]) / 2
            
            lbl = ax.text(mid_x, mid_y, f"{dist}", 
                          fontsize=10, color='red', ha='center', va='center', rotation=0, # Изменен цвет и fontsize
                          bbox=dict(boxstyle="round,pad=0.2", fc="white", ec="green", alpha=0.9))
            distance_labels.append(lbl)
            
    plt.show()


def main():
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.set_aspect('equal')
    
    draw_static_graph(ax)
    

if __name__ == '__main__':
    main()