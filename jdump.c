#include <systemd/sd-journal.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


int
main()
{
	int ret;
	sd_journal *journal;

	ret = sd_journal_open(&journal, SD_JOURNAL_LOCAL_ONLY);
	
	if (ret < 0)
	{
        fprintf(stderr, "Failed to open journal: %s\n", strerror(-ret));
        return 1;
    }

	SD_JOURNAL_FOREACH(journal)
	{
		print_fields(journal);
	}


	sd_journal_close(journal);
	return 0;
}
