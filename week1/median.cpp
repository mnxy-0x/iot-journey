#include <stdio.h>


typedef struct {
    float data[5];
    int count;
}  FloatWindow;

void insertion_sort(float* arr, int n) {
    for (int i = 1; i < n; ++i) {
        float key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// 3. Function taking Struct by "Reference" (Const Pointer in C)
float get_median_ref(FloatWindow& win) {
    
    float copy[5];
    for (int i = 0; i < win.count; ++i) {
        copy[i] = win.data[i];
    }
    
    insertion_sort(copy, win.count);
    
    return copy[win.count / 2]; 
}

// 4. Function taking Struct by POINTER (Mutable Pointer)
float get_median_ptr(FloatWindow* win) {
    
    if (win == nullptr || win->count == 0) {
        return 0.0f; 
    }
    
    float copy[5];
    for (int i = 0; i < win->count; ++i) {
        copy[i] = win->data[i];
    }
    
    insertion_sort(copy, win->count);
    
    return copy[win->count / 2];
}

int main() {

    FloatWindow window;
    window.count = 5;

    printf("Enter 5 float values: \n");
    for (int i = 0; i < 5; i++) {
        scanf("%f", &window.data[i]);
    }
    
    float med_ref = get_median_ref(window);
    float med_ptr = get_median_ptr(&window);
    
    printf("Median (by ref):        %.2f\n", med_ref);
    printf("Median (by pointer):    %.2f\n", med_ptr);

    return 0;
}