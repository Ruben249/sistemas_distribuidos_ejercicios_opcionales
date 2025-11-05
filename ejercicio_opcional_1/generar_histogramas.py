#!/usr/bin/env python3
"""
Histogram generator for mutex and semaphore latencies
Systems Distributed and Concurrentes
"""

import matplotlib.pyplot as plt
import numpy as np

def read_latency_data(filename):
    """Read latency data from file"""
    latencies = []
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                parts = line.strip().split()
                if len(parts) >= 2:
                    latencies.append(int(parts[1]))
    return np.array(latencies)

def calculate_statistics(data):
    """Calculate min, max, average, std dev from data"""
    min_val = np.min(data)
    max_val = np.max(data)
    avg = np.mean(data)
    std = np.std(data)
    count = len(data)
    return min_val, max_val, avg, std, count

def create_histogram():
    """Create latency histogram"""
    
    # Read latency data
    mutex_latencies = read_latency_data('mutex_temporal.dat')
    sem_latencies = read_latency_data('semaphore_temporal.dat')
    
    print(f"Loaded {len(mutex_latencies)} mutex measurements")
    print(f"Loaded {len(sem_latencies)} semaphore measurements")
    
    # Calculate statistics
    mutex_min, mutex_max, mutex_avg, mutex_std, mutex_count = calculate_statistics(mutex_latencies)
    sem_min, sem_max, sem_avg, sem_std, sem_count = calculate_statistics(sem_latencies)
    
    plt.figure(figsize=(12, 8))
    
    bins = 50
    plt.hist(mutex_latencies, bins=bins, alpha=0.7, color='blue', 
             label='Mutex', edgecolor='black', linewidth=0.5, density=False)
    plt.hist(sem_latencies, bins=bins, alpha=0.7, color='red', 
             label='Semaphore', edgecolor='black', linewidth=0.5, density=False)
    
    plt.xlabel('Latencia (nanosegundos)', fontsize=12, fontweight='bold')
    plt.ylabel('Frecuencia', fontsize=12, fontweight='bold')
    plt.title('Distribución de Latencia: Mutex vs Semáforo\n(60 segundos de medición, sistema idle)', 
              fontsize=14, fontweight='bold', pad=20)
    
    plt.legend(fontsize=11, framealpha=0.9)
    plt.grid(True, alpha=0.3, linestyle='--', axis='y')
    
    plt.ylim(0, 60000)
    plt.xlim(0, 30000)
    
    stats_text = (f'Estadísticas:\n'
                  f'Mutex: {mutex_count} muestras\n'
                  f'Mín: {mutex_min:.1f} ns, Máx: {mutex_max:.1f} ns\n'
                  f'Promedio: {mutex_avg:.1f} ns, Std: {mutex_std:.1f} ns\n\n'
                  f'Semáforo: {sem_count} muestras\n'
                  f'Mín: {sem_min:.1f} ns, Máx: {sem_max:.1f} ns\n'
                  f'Promedio: {sem_avg:.1f} ns, Std: {sem_std:.1f} ns')
    
    plt.text(0.02, 0.98, stats_text, transform=plt.gca().transAxes,
             verticalalignment='top', horizontalalignment='left',
             bbox=dict(boxstyle='round', facecolor='lightgray', alpha=0.8),
             fontsize=9, fontfamily='monospace')
    
    # Adjust layout
    plt.tight_layout()
    plt.show()
    
    print(f"Datos originales: {mutex_count} muestras mutex, {sem_count} muestras semáforo")

if __name__ == "__main__":
    create_histogram()