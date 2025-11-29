import matplotlib.pyplot as plt
import matplotlib.animation as animation
import math
import os
import time

FILE_NAME = "aco_data.txt"

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

def main():
    print(f"Ожидание данных из {FILE_NAME}...")
    
    raw_data = []
    while not raw_data:
        try:
            raw_data = load_data(FILE_NAME)
            if not raw_data:
                time.sleep(1)
        except:
            time.sleep(1)
            
    print(f"Загружено {len(raw_data)} шагов оптимизации.")
    
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.5)

    scat = ax.scatter([], [], color='blue', s=100, zorder=5)
    line, = ax.plot([], [], color='gray', linestyle='-', linewidth=1.5, zorder=2)
    
    coords_text = []
    dist_labels = [] 

    init_coords = raw_data[0]['COORDS']
    all_x = [c[0] for c in init_coords]
    all_y = [c[1] for c in init_coords]
    margin = 10
    ax.set_xlim(min(all_x) - margin, max(all_x) + margin)
    ax.set_ylim(min(all_y) - margin, max(all_y) + margin)

    ani = animation.FuncAnimation(
        fig, 
        update_plot, 
        frames=len(raw_data), 
        fargs=(raw_data, scat, line, coords_text, dist_labels, ax),
        interval=800,
        repeat=True
    )

    plt.show()

if __name__ == '__main__':
    main()