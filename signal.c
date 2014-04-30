// Minimal example for signal handler.

#include <stdio.h>
#include <signal.h>

// Signal handler function.
void sigusr_handler(int sig) {
  printf("Handler: signal %i received.\n", sig);
  return;
}

int main() {
  // Action struct to divert signal.
  struct sigaction sigusr_action;
  // Mask for blocking signal while handler is running: can be empty.
  sigset_t block_mask;
  sigemptyset (&block_mask);
  // Choose handler to be a function.
  sigusr_action.sa_handler = sigusr_handler;
  // Or this is how you ignore the signal.
  //sigusr_action.sa_handler = SIG_IGN;
  // Set mask and flags.
  sigusr_action.sa_mask = block_mask;
  sigusr_action.sa_flags = 0;
  // Set signal action.
  sigaction(SIGUSR1, &sigusr_action, NULL);

  // Print statements to demonstrate control flow.
  printf("Before signal.\n");
  raise(SIGUSR1);
  printf("After signal.\n");

  // Done.
  return 0;
}
