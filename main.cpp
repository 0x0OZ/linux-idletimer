#include <iostream>
#include <csignal>

// Common function to handle signals
void handleSignal(int signal)
{

    std::cout << "Received signal: " << signal << std::endl;
    // Add your common handling logic here
}
int main()
{
    // Set up signal handlers for the signals of interest
    signal(SIGTERM, handleSignal);
    signal(SIGHUP, handleSignal);
    signal(SIGINT, handleSignal);
    signal(SIGABRT, handleSignal);
    signal(SIGPIPE, handleSignal);
    signal(SIGUSR1, handleSignal);
    signal(SIGUSR2, handleSignal);
    signal(SIGCHLD, handleSignal);
    signal(SIGTSTP, handleSignal);
    signal(SIGCONT, handleSignal);
    signal(SIGTTIN, handleSignal);
    signal(SIGTTOU, handleSignal);

    // Your program logic here

    // For demonstration, let's simulate some signals being sent
    std::cout << "Simulating signals..." << std::endl;
    raise(SIGTERM);

    // Your program continues running

    return 0;
}
