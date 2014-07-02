#include <systemd/sd-journal.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define INFINITE_WAIT -1
#define LAST_MESSAGE_NUMBER 10

int
main()
{
	int ret;
	int i;
	sd_journal *journal;

	ret = sd_journal_open(&journal, SD_JOURNAL_LOCAL_ONLY);
	
	if (ret < 0)
	{
        fprintf(stderr, "Failed to open journal: %s\n", strerror(-ret));
        return 1;
    }

	ret = sd_journal_seek_tail(journal);

	if (ret < 0)
	{
		fprintf(stderr, "Failed to seek to the tail of the journal: %s\n", strerror(-ret));
        return 1;
	}

	for (i = 0; i < LAST_MESSAGE_NUMBER; i++)
	{
		ret = sd_journal_previous(journal);
	}

	while ((ret = sd_journal_wait(journal, INFINITE_WAIT)) >= 0)
	{
		for (;;) 
		{
			ret = sd_journal_next(journal);
			
			/* no more message, go to sleep */
			if (ret == 0)
			{
				break;
			}
			else if (ret < 0)			
			{
				fprintf(stderr, "Failed to seek to the tail of the journal: %s\n", strerror(-ret));
        		return 1;
			}
			
			print_fields(journal);
			printf("--------------------------------------------------------\n");
		}	
	}

	sd_journal_close(journal);
	return 0;
}
