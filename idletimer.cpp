#include <libevdev-1.0/libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>
#include <iostream>
#include <cstdio>

size_t deviceCount = 0;
libevdev *devices[16]; // Assuming a maximum of 16 input devices
time_t lastSeen = time(nullptr);

bool initializeLinuxInputDevices(libevdev **devices, size_t *deviceCount)
{
    // Use glob to enumerate input devices in /dev/input/
    glob_t globbuf;
    if (glob("/dev/input/event*", GLOB_NOSORT, nullptr, &globbuf) != 0)
    {
        fprintf(stderr, "Failed to enumerate input devices.\n");
        fflush(stderr);
        return false;
    }

    *deviceCount = globbuf.gl_pathc;
    // Open and initialize each device
    for (size_t i = 0; i < *deviceCount; ++i)
    {
        const char *devicePath = globbuf.gl_pathv[i];
        int fd = open(devicePath, O_RDONLY | O_NONBLOCK);
        if (fd < 0)
        {
            fprintf(stderr, "Failed to open device: %s\n", devicePath);
            fflush(stderr);

            return false;
        }

        if (libevdev_new_from_fd(fd, &devices[i]) < 0)
        {
            fprintf(stderr, "Failed to initialize libevdev for device: %s\n", devicePath);
            fflush(stderr);
            return false;
        }
    }

    // Free the glob buffer
    globfree(&globbuf);

    return true;
}

// Function to check input events and calculate idle time
long checkLinuxInputEventsAndGetIdleTime(libevdev **devices, size_t deviceCount, time_t *lastSeen)
{
    long idleTime = 0;
    bool systemInUse = false;

    // Read events from all devices
    for (size_t i = 0; i < deviceCount; ++i)
    {
        struct input_event ev;
        int rc;

        while ((rc = libevdev_next_event(devices[i], LIBEVDEV_READ_FLAG_NORMAL, &ev)) >= 0)
        {
            // Set systemInUse to true if an event is detected
            systemInUse = true;
        }

        if (rc < 0 && rc != -EAGAIN)
        {
            return 0;
        }
    }

    if (systemInUse)
    {
        *lastSeen = time(nullptr);
        idleTime = 0;
    }
    else
    {
        idleTime = time(nullptr) - *lastSeen;
    }

    return idleTime;
}

void cleanupLinuxInputDevices(libevdev **devices, size_t deviceCount)
{
    // Close and free libevdev structures
    for (size_t i = 0; i < deviceCount; ++i)
    {
        libevdev_free(devices[i]);
    }
}

int main()
{

    if (!initializeLinuxInputDevices(devices, &deviceCount))
    {
        return 1;
    }

    // Main loop to monitor input events and calculate idle time
    fprintf(stdout, "sleep for 1 before start");
    fflush(stdout);
    sleep(1);
    while (true)
    {
        long idleTime = checkLinuxInputEventsAndGetIdleTime(devices, deviceCount, &lastSeen);

        // You can use the 'idleTime' value as needed
        fprintf(stdout, "\033[2J\033[1;1H");
        fprintf(stdout, "Idle time: %ld seconds\r", idleTime);
        fflush(stdout);
        sleep(1);
    }

    // Clean up and close devices (this part may not be reached)
    cleanupLinuxInputDevices(devices, deviceCount);
    return 0;
}
