# Create a Cron Task

task.c - a program that collect the following statistics using system calls:

- Current Process ID
- Current User ID
- Current date and Time
- Output of a Customized System Call

### Put this program onto Cron list for periodic execution
    # Build the program
    gcc task.c -o test
    # Open the crontab file to add task
    crontab -e
    # Append the line to the end of the file:
    0,10,20,30,40,50 * * * * cd $(file_path) && ./test >> $(file_path)/cron.log
