







Pusedo Code


Loop forever:
    Display a prompt
    Read user input
    Parse input into command and arguments
    Fork a new process
    In the child process:
        Execute the command using execvp()
    In the parent process:
        Wait for the child process to finish
    Eof end the function 
