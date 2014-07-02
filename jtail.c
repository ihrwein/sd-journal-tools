#include <systemd/sd-journal.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define INFINITE_WAIT -1
#define LAST_MESSAGE_NUMBER 10

void
seek_back_by_n(sd_journal* journal, int n)
{
	int i;
	int ret;

	for (i = 0; i < LAST_MESSAGE_NUMBER; i++)
	{
		ret = sd_journal_previous(journal);
		print_err("Failed to seek back in the journal: %s\n", ret);
	}
}

int
main()
{
	int ret;
	sd_journal *journal;

	ret = sd_journal_open(&journal, SD_JOURNAL_LOCAL_ONLY);
	
	ret = sd_journal_seek_tail(journal);
	ret = print_err("Failed to seek to the tail of the journal: %s\n", ret);
	
	if (ret)
		return 1;

	seek_back_by_n(journal, LAST_MESSAGE_NUMBER);	

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
