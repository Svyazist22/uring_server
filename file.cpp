#include "file.h"

bool File::open (const char* filename)
{
    file.open (filename, std::ios::app);
    if (!file.is_open ())
    {
        printf ("ERROR: don't open file!");
        return false;
    }
    return true;
}

bool File::write (char* data, size_t size)
{
    if (!file.is_open ())
        return false;
    
    file.write (data, size);
    return true;
}

void File::close ()
{
    if (file.is_open ())
        file.close ();
}
