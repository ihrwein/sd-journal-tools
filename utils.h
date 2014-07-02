#ifndef UTILS_H
#define UTILS_H 1

void
print_fields(sd_journal *j)
{
    const void *data;
    size_t length;
    SD_JOURNAL_FOREACH_DATA(j, data, length)
    {
        printf("%.*s\n", (int) length, (char*) data);
    }
}

#endif
