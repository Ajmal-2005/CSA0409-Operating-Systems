#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
 
#define N 5
#define REF_LEN 14
#define FRAMES 3
#define DISK_REQS 8
#define MAX_SEGMENTS 50
 
typedef struct {
    char id[4];
    int at;
    int bt;
    int priority;
    int ct;
    int wt;
    int tat;
    int remaining;
    int completed;
} Process;
 
typedef struct {
    char id[4];
    int start;
    int end;
} Segment;
 
/* ---------------------------------------------------------
   Utility Functions
   --------------------------------------------------------- */
 
void copy_processes(Process dst[], const Process src[]) {
    for (int i = 0; i < N; i++)
        dst[i] = src[i];
}
 
void print_cpu_metrics(Process p[]) {
    double total_wt = 0.0, total_tat = 0.0;
 
    printf("\n%-8s %-5s %-5s %-8s %-8s %-8s\n",
           "Process", "AT", "BT", "CT", "WT", "TAT");
    printf("-------------------------------------------------\n");
 
    for (int i = 0; i < N; i++) {
        printf("%-8s %-5d %-5d %-8d %-8d %-8d\n",
               p[i].id, p[i].at, p[i].bt,
               p[i].ct, p[i].wt, p[i].tat);
 
        total_wt += p[i].wt;
        total_tat += p[i].tat;
    }
 
    printf("Average Waiting Time    = %.2f\n", total_wt / N);
    printf("Average Turnaround Time = %.2f\n", total_tat / N);
}
 
void print_gantt(Segment seg[], int count) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < count; i++) {
        printf("| %s ", seg[i].id);
    }
    printf("|\n");
 
    printf("%d", seg[0].start);
    for (int i = 0; i < count; i++) {
        printf("%8d", seg[i].end);
    }
    printf("\n");
}
 
/* ---------------------------------------------------------
   FCFS CPU Scheduling
   --------------------------------------------------------- */
 
void fcfs_cpu(const Process base[]) {
    Process p[N];
    Segment seg[MAX_SEGMENTS];
    int seg_count = 0;
    int time = 0;
 
    copy_processes(p, base);
 
    for (int i = 0; i < N; i++) {
        if (time < p[i].at)
            time = p[i].at;
 
        seg[seg_count].start = time;
        seg[seg_count].end = time + p[i].bt;
        snprintf(seg[seg_count].id, sizeof(seg[seg_count].id),
                 "%.3s", p[i].id);
        seg_count++;
 
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }
 
    printf("\n================ FCFS CPU ================\n");
    print_gantt(seg, seg_count);
    print_cpu_metrics(p);
}
 
/* ---------------------------------------------------------
   SJF Non-Preemptive
   --------------------------------------------------------- */
 
void sjf_cpu(const Process base[]) {
    Process p[N];
    Segment seg[MAX_SEGMENTS];
    int done[N] = {0};
    int completed = 0;
    int time = 0;
    int seg_count = 0;
 
    copy_processes(p, base);
 
    while (completed < N) {
        int idx = -1;
 
        for (int i = 0; i < N; i++) {
            if (!done[i] && p[i].at <= time) {
                if (idx == -1 ||
                    p[i].bt < p[idx].bt ||
                    (p[i].bt == p[idx].bt && p[i].at < p[idx].at)) {
                    idx = i;
                }
            }
        }
 
        if (idx == -1) {
            time++;
            continue;
        }
 
        seg[seg_count].start = time;
        seg[seg_count].end = time + p[idx].bt;
        snprintf(seg[seg_count].id, sizeof(seg[seg_count].id),
                 "%.3s", p[idx].id);
        seg_count++;
 
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
 
        done[idx] = 1;
        completed++;
    }
 
    printf("\n================ SJF CPU ================\n");
    print_gantt(seg, seg_count);
    print_cpu_metrics(p);
}
 
/* ---------------------------------------------------------
   Priority Non-Preemptive
   Smaller priority number = higher priority
   --------------------------------------------------------- */
 
void priority_cpu(const Process base[]) {
    Process p[N];
    Segment seg[MAX_SEGMENTS];
    int done[N] = {0};
    int completed = 0;
    int time = 0;
    int seg_count = 0;
 
    copy_processes(p, base);
 
    while (completed < N) {
        int idx = -1;
 
        for (int i = 0; i < N; i++) {
            if (!done[i] && p[i].at <= time) {
                if (idx == -1 ||
                    p[i].priority < p[idx].priority ||
                    (p[i].priority == p[idx].priority &&
                     p[i].at < p[idx].at)) {
                    idx = i;
                }
            }
        }
 
        if (idx == -1) {
            time++;
            continue;
        }
 
        seg[seg_count].start = time;
        seg[seg_count].end = time + p[idx].bt;
        snprintf(seg[seg_count].id, sizeof(seg[seg_count].id),
                 "%.3s", p[idx].id);
        seg_count++;
 
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
 
        done[idx] = 1;
        completed++;
    }
 
    printf("\n============ PRIORITY CPU ============\n");
    printf("Priority rule: lower number = higher priority\n");
    print_gantt(seg, seg_count);
    print_cpu_metrics(p);
}
 
/* ---------------------------------------------------------
   Round Robin CPU Scheduling
   --------------------------------------------------------- */
 
void rr_cpu(const Process base[], int quantum) {
    Process p[N];
    Segment seg[MAX_SEGMENTS];
 
    int queue[100];
    int front = 0, rear = 0;
    int added[N] = {0};
 
    int time = 0;
    int completed = 0;
    int seg_count = 0;
 
    copy_processes(p, base);
 
    for (int i = 0; i < N; i++) {
        p[i].remaining = p[i].bt;
        p[i].completed = 0;
    }
 
    while (completed < N) {
        /* Add all processes that have arrived */
        for (int i = 0; i < N; i++) {
            if (!added[i] && p[i].at <= time) {
                queue[rear++] = i;
                added[i] = 1;
            }
        }
 
        /* CPU idle */
        if (front == rear) {
            int next_arrival = INT_MAX;
 
            for (int i = 0; i < N; i++) {
                if (!added[i] && p[i].at < next_arrival)
                    next_arrival = p[i].at;
            }
 
            if (next_arrival != INT_MAX)
                time = next_arrival;
 
            continue;
        }
 
        int idx = queue[front++];
 
        int run = p[idx].remaining < quantum
                    ? p[idx].remaining
                    : quantum;
 
        seg[seg_count].start = time;
        seg[seg_count].end = time + run;
        snprintf(seg[seg_count].id, sizeof(seg[seg_count].id),
                 "%.3s", p[idx].id);
        seg_count++;
 
        time += run;
        p[idx].remaining -= run;
 
        /* Add new arrivals that occurred during this slice */
        for (int i = 0; i < N; i++) {
            if (!added[i] && p[i].at <= time) {
                queue[rear++] = i;
                added[i] = 1;
            }
        }
 
        if (p[idx].remaining > 0) {
            queue[rear++] = idx;
        } else {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            p[idx].completed = 1;
            completed++;
        }
    }
 
    printf("\n============ ROUND ROBIN CPU ============\n");
    printf("Time Quantum = %d\n", quantum);
    print_gantt(seg, seg_count);
    print_cpu_metrics(p);
}
 
/* ---------------------------------------------------------
   Page Replacement
   --------------------------------------------------------- */
 
int page_in_frames(int frames[], int n, int page) {
    for (int i = 0; i < n; i++) {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}
 
int fifo_page_faults(const int ref[], int len, int frame_count) {
    int frames[FRAMES];
    int pointer = 0;
    int faults = 0;
 
    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;
 
    for (int i = 0; i < len; i++) {
        if (!page_in_frames(frames, frame_count, ref[i])) {
            faults++;
            frames[pointer] = ref[i];
            pointer = (pointer + 1) % frame_count;
        }
    }
 
    return faults;
}
 
int find_lru_victim(int frames[], int frame_count,
                    const int ref[], int current_index) {
    int victim = -1;
    int least_recent = INT_MAX;
 
    for (int i = 0; i < frame_count; i++) {
        int last_used = -1;
 
        for (int j = current_index - 1; j >= 0; j--) {
            if (ref[j] == frames[i]) {
                last_used = j;
                break;
            }
        }
 
        if (last_used < least_recent) {
            least_recent = last_used;
            victim = i;
        }
    }
 
    return victim;
}
 
int lru_page_faults(const int ref[], int len, int frame_count) {
    int frames[FRAMES];
    int filled = 0;
    int faults = 0;
 
    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;
 
    for (int i = 0; i < len; i++) {
        if (page_in_frames(frames, filled, ref[i])) {
            continue;
        }
 
        faults++;
 
        if (filled < frame_count) {
            frames[filled++] = ref[i];
        } else {
            int victim = find_lru_victim(
                frames, frame_count, ref, i
            );
            frames[victim] = ref[i];
        }
    }
 
    return faults;
}
 
int next_use(const int ref[], int len, int current_index, int page) {
    for (int i = current_index + 1; i < len; i++) {
        if (ref[i] == page)
            return i;
    }
    return INT_MAX;
}
 
int optimal_page_faults(const int ref[], int len, int frame_count) {
    int frames[FRAMES];
    int filled = 0;
    int faults = 0;
 
    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;
 
    for (int i = 0; i < len; i++) {
        if (page_in_frames(frames, filled, ref[i])) {
            continue;
        }
 
        faults++;
 
        if (filled < frame_count) {
            frames[filled++] = ref[i];
        } else {
            int victim = -1;
            int farthest = -1;
 
            for (int f = 0; f < frame_count; f++) {
                int next = next_use(ref, len, i, frames[f]);
 
                if (next > farthest) {
                    farthest = next;
                    victim = f;
                }
            }
 
            frames[victim] = ref[i];
        }
    }
 
    return faults;
}
 
void page_replacement_demo(const int ref[], int len, int frame_count) {
    int fifo = fifo_page_faults(ref, len, frame_count);
    int lru = lru_page_faults(ref, len, frame_count);
    int opt = optimal_page_faults(ref, len, frame_count);
 
    printf("\n========== PAGE REPLACEMENT ==========\n");
 
    printf("Reference string: ");
    for (int i = 0; i < len; i++)
        printf("%d%s", ref[i], i == len - 1 ? "\n" : ", ");
 
    printf("Frames = %d\n\n", frame_count);
 
    printf("%-12s %-12s\n", "Algorithm", "Page Faults");
    printf("--------------------------\n");
    printf("%-12s %-12d\n", "FIFO", fifo);
    printf("%-12s %-12d\n", "LRU", lru);
    printf("%-12s %-12d\n", "Optimal", opt);
}
 
/* ---------------------------------------------------------
   Disk Scheduling
   --------------------------------------------------------- */
 
int abs_int(int x) {
    return x < 0 ? -x : x;
}
 
int disk_fcfs(const int queue[], int n, int head) {
    int movement = 0;
    int current = head;
 
    for (int i = 0; i < n; i++) {
        movement += abs_int(queue[i] - current);
        current = queue[i];
    }
 
    return movement;
}
 
int disk_sstf(const int queue[], int n, int head) {
    int served[DISK_REQS] = {0};
    int current = head;
    int movement = 0;
 
    for (int count = 0; count < n; count++) {
        int best = -1;
        int best_distance = INT_MAX;
 
        for (int i = 0; i < n; i++) {
            if (!served[i]) {
                int distance = abs_int(queue[i] - current);
 
                if (distance < best_distance ||
                    (distance == best_distance &&
                     queue[i] < queue[best])) {
                    best_distance = distance;
                    best = i;
                }
            }
        }
 
        served[best] = 1;
        movement += abs_int(queue[best] - current);
        current = queue[best];
    }
 
    return movement;
}
 
void sort_int(int a[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[i]) {
                int temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
    }
}
 
int disk_scan(const int queue[], int n, int head, int max_cylinder) {
    int lower[DISK_REQS], upper[DISK_REQS];
    int low_n = 0, high_n = 0;
    int current = head;
    int movement = 0;
 
    for (int i = 0; i < n; i++) {
        if (queue[i] < head)
            lower[low_n++] = queue[i];
        else
            upper[high_n++] = queue[i];
    }
 
    sort_int(lower, low_n);
    sort_int(upper, high_n);
 
    /* Move upward first */
    for (int i = 0; i < high_n; i++) {
        movement += abs_int(upper[i] - current);
        current = upper[i];
    }
 
    /* Reach the end */
    movement += abs_int(max_cylinder - current);
    current = max_cylinder;
 
    /* Reverse direction */
    for (int i = low_n - 1; i >= 0; i--) {
        movement += abs_int(lower[i] - current);
        current = lower[i];
    }
 
    return movement;
}
 
int disk_cscan(const int queue[], int n, int head, int max_cylinder) {
    int lower[DISK_REQS], upper[DISK_REQS];
    int low_n = 0, high_n = 0;
    int current = head;
    int movement = 0;
 
    for (int i = 0; i < n; i++) {
        if (queue[i] < head)
            lower[low_n++] = queue[i];
        else
            upper[high_n++] = queue[i];
    }
 
    sort_int(lower, low_n);
    sort_int(upper, high_n);
 
    /* Move upward */
    for (int i = 0; i < high_n; i++) {
        movement += abs_int(upper[i] - current);
        current = upper[i];
    }
 
    /* Reach maximum cylinder */
    movement += abs_int(max_cylinder - current);
    current = max_cylinder;
 
    /* Circular jump to cylinder 0 */
    movement += max_cylinder;
    current = 0;
 
    /* Continue upward */
    for (int i = 0; i < low_n; i++) {
        movement += abs_int(lower[i] - current);
        current = lower[i];
    }
 
    return movement;
}
 
void disk_demo(void) {
    int queue[DISK_REQS] = {
        20, 160, 45, 130, 75, 10, 150, 95
    };
 
    int head = 90;
    int max_cylinder = 180;
 
    printf("\n============== DISK SCHEDULING ==============\n");
 
    printf("Request queue: ");
    for (int i = 0; i < DISK_REQS; i++)
        printf("%d%s", queue[i],
               i == DISK_REQS - 1 ? "\n" : ", ");
 
    printf("Initial head = %d\n", head);
    printf("Cylinders = 0-%d\n", max_cylinder);
    printf("SCAN direction = upward\n\n");
 
    int fcfs = disk_fcfs(queue, DISK_REQS, head);
    int sstf = disk_sstf(queue, DISK_REQS, head);
    int scan = disk_scan(queue, DISK_REQS, head, max_cylinder);
    int cscan = disk_cscan(queue, DISK_REQS, head, max_cylinder);
 
    printf("%-12s %-20s\n", "Algorithm", "Head Movement");
    printf("------------------------------------\n");
    printf("%-12s %-20d\n", "FCFS", fcfs);
    printf("%-12s %-20d\n", "SSTF", sstf);
    printf("%-12s %-20d\n", "SCAN", scan);
    printf("%-12s %-20d\n", "C-SCAN", cscan);
}
 
/* ---------------------------------------------------------
   Main
   --------------------------------------------------------- */
 
int main(void) {
    Process base[N] = {
        {"P1", 0, 3, 2, 0, 0, 0, 0, 0},
        {"P2", 2, 6, 3, 0, 0, 0, 0, 0},
        {"P3", 4, 2, 1, 0, 0, 0, 0, 0},
        {"P4", 6, 8, 4, 0, 0, 0, 0, 0},
        {"P5", 8, 4, 2, 0, 0, 0, 0, 0}
    };
 
    int reference_string[REF_LEN] = {
        1, 2, 3, 1, 4, 5, 2,
        1, 3, 5, 4, 2, 1, 5
    };
 
    printf("=============================================\n");
    printf(" BANK TELLER / ATM TRANSACTION SERVER\n");
    printf(" OS RESOURCE MANAGEMENT SIMULATION IN C\n");
    printf("=============================================\n");
 
    /* CPU Scheduling */
    fcfs_cpu(base);
    sjf_cpu(base);
    priority_cpu(base);
    rr_cpu(base, 3);
 
    /* Memory */
    page_replacement_demo(
        reference_string, REF_LEN, FRAMES
    );
 
    /* Disk */
    disk_demo();
 
    printf("\n=============================================\n");
    printf("Simulation complete.\n");
    printf("=============================================\n");
 
    return 0;
}