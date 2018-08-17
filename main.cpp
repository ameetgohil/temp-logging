// Temperature logging demo - record temperatures to SD card and print them to the console every 10 seconds

#include "mbed.h"
#include "SDFileSystem.h"

// Analog input (pin 15)
AnalogIn ain(p15);

// USB serial (tx,rx)
Serial pc(USBTX, USBRX);

// SD card (SPI pins)
SDFileSystem sd(p5, p6, p7, p8, "sd");

// Timer for our timestamps
Timer timer;

int main() {
    
    FILE *file;
    float voltage_in;
    float degrees_c;
    int i;
    int c;
    
    // Start our timer
    timer.start();
    
    // Open file for writing
    file = fopen("/sd/temp_data.txt","w");
    if ( file == NULL ) {
        error("ERROR: Could not open file for writing!\n\r");
        return -1;
    }
    
    // Tell the user we need to wait whille we colleect some data
    pc.printf("\nCollecting data (Do not remove SD Card!) ...\n\r");
    
    // Collect temperatures with timestamps every second
    for(i=0;i<10;i++) {
        voltage_in = ain * 3.3;
        degrees_c = (voltage_in - 0.5) * 100.0;
        fprintf(file, "%2.2fs: %3.1f deg C\n\r",timer.read(), degrees_c);
        wait(1);
    }
    
    // Close file and re-open it for reading
    fclose(file);
    file = fopen("/sd/temp_data.txt","r");
    if ( file == NULL ) {
        error("ERROR: Could not open file for reading!\n\r");
        return -1;
    }
    
    // Print results to console
    pc.printf("Temperature data:\n\r");
    while(1) {
        c = fgetc(file);
        if ( c == EOF ) {
            break;
        }
        pc.putc(c);
    }
    
    // Close the file and finish
    fclose(file);
    pc.printf("Done! Safe to remove SD card\n\r");
    
    return 0;
}
