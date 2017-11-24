#include "types.h"
#include "stat.h"
#include "user.h"
#define passwd_file "/passwd"       // Point to 'passwd' file
#define max_length 100
#define name_lenght 50

int contains( char *str, char *pattern )
{
    int i = 0, j = 0;
    int size_str = strlen(str);
    int size_pattern = strlen(pattern);

    while ( i < size_pattern && j < size_str && pattern[i] != str[j] )
    {
        j++;
    }
    if ( j == size_str ) 
    {
        return 0;
    }
    while ( i < size_pattern && j < size_str && pattern[i] == str[j] )
    {
        i++;
        j++;
    }
    if ( i == size_pattern ) 
    {
        return 1;
    } 
    else
    {
        return 0;
    }
}

int match( char *str, char *user, char *pass )
{
    int i = 0, j = 0;
    int size_str = strlen(str);

    while ( str[i] != ':' ) 
    {
        if ( str[i] != user[i] ) {
            return 0;
        }
        i++; 
    }
    i++;
    while ( str[i] != ':' ) i++;
    i++;
    while ( i < size_str ) 
    {
        if ( str[i] != pass[j] ) {
            return 0;
        }
        i++;
        j++;
    }
    return 1;
}

int readLine( int fd, char *buf, int size )
{
    int i = 0, n = 0;
    char c;
    while ( read( fd, &c, 1 ) > 0) 
    {
        n++;
        if ( c != '\n' )
        {
            buf[i] = c;
            i++;
            if ( i == size - 1)
            {
                break;
            }
        }
        else {
            break;
        }
    } 
    if(buf[i-1] == '\r')
        buf[i-1] = '\0';
    else
        buf[i] = '\0';
    return n;
}

int main( int argc, char *argv[] )
{
    int fd;                                             // File descriptor
    char record[max_length];
    char user_name[name_lenght];
    char user_password[name_lenght];
    int n, pid, wpid;
    char *shArgv[] = { "sh", 0 };

    while(1) 
    {
        printf( 1, "login: ");
        read( 0, user_name, sizeof(user_name) );
        printf( 1, "password: ");
        read( 0, user_password, sizeof(user_password) );
    
        user_name[strlen(user_name) - 1] = '\0';            // Delete '\n' form input
        user_password[strlen(user_password) - 1] = '\0';    // Delete '\n' form input
    
        fd = open( passwd_file, 0);                         // Open file with write permissions only
        while ( ( n = readLine(fd, record, sizeof(record)) ) > 0 ) 
        {
            if ( match(record, user_name, user_password) )
            {
                pid = fork();
                if(pid < 0){
                  printf(1, "login: fork failed\n");
                  exit();
                }
                if(pid == 0){
                  exec("sh", shArgv);
                  printf(1, "init: exec sh failed\n");
                  exit();
                }
                while((wpid=wait()) >= 0 && wpid != pid)
                  printf(1, "zombie!\n");
                //
            }
            
        }
        printf( 1, "Log in failed, wrong username or password\n");
        close(fd);
    }
    exit();
}