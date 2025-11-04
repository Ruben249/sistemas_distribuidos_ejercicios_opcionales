#!/usr/bin/env python3
"""
Histogram generator for mutex and semaphore latencies
Systems Distributed and Concurrentes
"""

import matplotlib.pyplot as plt
import numpy as np

def read_temporal_data(filename):
    """Read temporal latency data from file"""
    measurements = []
    latencies = []
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                parts = line.strip().split()
                if len(parts) >= 2:
                    measurements.append(int(parts[0]))
                    latencies.append(int(parts[1]))
    return np.array(measurements), np.array(latencies)

def calculate_statistics(data):
    """Calculate min, max, average, std dev from data"""
    min_val = np.min(data)
    max_val = np.max(data)
    avg = np.mean(data)
    std = np.std(data)
    count = len(data)
    return min_val, max_val, avg, std, count

def average_by_chunks(measurements, latencies, chunk_size=1000):
    """Average data by chunks of specified size"""
    n_chunks = len(measurements) // chunk_size
    avg_measurements = []
    avg_latencies = []
    
    for i in range(n_chunks):
        start_idx = i * chunk_size
        end_idx = (i + 1) * chunk_size
        
        chunk_measurements = measurements[start_idx:end_idx]
        chunk_latencies = latencies[start_idx:end_idx]
        
        avg_measurements.append(np.mean(chunk_measurements))
        avg_latencies.append(np.mean(chunk_latencies))
    
    # Handle remaining measurements if any
    if len(measurements) % chunk_size != 0:
        start_idx = n_chunks * chunk_size
        avg_measurements.append(np.mean(measurements[start_idx:]))
        avg_latencies.append(np.mean(latencies[start_idx:]))
    
    return np.array(avg_measurements), np.array(avg_latencies)

def create_latency_distribution():
    """Create latency distribution plot from temporal data"""
    
    # Read temporal data
    mutex_measurements, mutex_latencies = read_temporal_data('mutex_temporal.dat')
    sem_measurements, sem_latencies = read_temporal_data('semaphore_temporal.dat')
    
    print(f"Loaded {len(mutex_measurements)} mutex measurements")
    print(f"Loaded {len(sem_measurements)} semaphore measurements")
    
    # Average data by chunks of 1000
    chunk_size = 1000
    mutex_avg_meas, mutex_avg_lat = average_by_chunks(mutex_measurements, mutex_latencies, chunk_size)
    sem_avg_meas, sem_avg_lat = average_by_chunks(sem_measurements, sem_latencies, chunk_size)
    
    print(f"After averaging: {len(mutex_avg_meas)} mutex points, {len(sem_avg_meas)} semaphore points")
    
    # Calculate statistics from original data
    mutex_min, mutex_max, mutex_avg, mutex_std, mutex_count = calculate_statistics(mutex_latencies)
    sem_min, sem_max, sem_avg, sem_std, sem_count = calculate_statistics(sem_latencies)
    
    # Create plot
    plt.figure(figsize=(14, 8))
    
    # Plot averaged data as bars
    bar_width = 300  # Ancho de las barras
    plt.bar(mutex_avg_meas - bar_width/2, mutex_avg_lat, width=bar_width, 
            label='Mutex', alpha=0.7, color='blue', edgecolor='black')
    plt.bar(sem_avg_meas + bar_width/2, sem_avg_lat, width=bar_width, 
            label='Semaphore', alpha=0.7, color='red', edgecolor='black')
    
    # Customize plot
    plt.xlabel('Measurement Number (averaged every 1000 samples)', fontsize=12, fontweight='bold')
    plt.ylabel('Latency (nanoseconds)', fontsize=12, fontweight='bold')
    plt.title('Latency Distribution: Mutex vs Semaphore\n(60 seconds measurement, idle system)', 
              fontsize=14, fontweight='bold', pad=20)
    
    plt.legend(fontsize=11, framealpha=0.9, loc='upper right')
    plt.grid(True, alpha=0.3, linestyle='--')
    
    stats_text = (f'Statistics (from all {mutex_count} samples):\n'
                  f'Mutex:\n'
                  f'Min: {mutex_min:.1f} ns, Max: {mutex_max:.1f} ns\n'
                  f'Avg: {mutex_avg:.1f} ns, Std: {mutex_std:.1f} ns\n\n'
                  f'Semaphore:\n'
                  f'Min: {sem_min:.1f} ns, Max: {sem_max:.1f} ns\n'
                  f'Avg: {sem_avg:.1f} ns, Std: {sem_std:.1f} ns')
    
    plt.text(0.02, 0.98, stats_text, transform=plt.gca().transAxes,
             verticalalignment='top', horizontalalignment='left',
             bbox=dict(boxstyle='round', facecolor='lightgray', alpha=0.8),
             fontsize=9, fontfamily='monospace')
    
    # Adjust y-axis to focus on main distribution (optional)
    plt.ylim(0, 1500)
    
    # Add note about averaging
    plt.text(0.98, 0.02, f'Note: Data averaged every {chunk_size} samples\nfor better visualization', 
             transform=plt.gca().transAxes, verticalalignment='bottom', 
             horizontalalignment='right', fontsize=8, style='italic',
             bbox=dict(boxstyle='round', facecolor='white', alpha=0.7))
    
    # Adjust layout
    plt.tight_layout()
    
    plt.show()

if __name__ == "__main__":
    create_latency_distribution()