#include <systemd/sd-journal.h>
#include <sys/poll.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

#define INFINITE_WAIT -1
#define LAST_MESSAGE_NUMBER 10
#define POLL 0

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
journal_wait(sd_journal* journal)
{
#if POLL
  struct pollfd pollfd;

  pollfd.fd = sd_journal_get_fd(journal);
  pollfd.events = sd_journal_get_events(journal);
  poll(&pollfd, 1, INFINITE_WAIT);
  return sd_journal_process(journal);
#else
  return sd_journal_wait(journal, INFINITE_WAIT);
#endif
}

void
process_available_messages(sd_journal* journal)
{
  int ret;

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
          fprintf(stderr, "Failed to read the next message: %s\n", strerror(-ret));
          return;
        }

      print_fields(journal);
      printf("--------------------------------------------------------\n");
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

  process_available_messages(journal);

  while ((ret = journal_wait(journal)) >= 0)
    {
      process_available_messages(journal);
    }

  sd_journal_close(journal);
  return 0;
}
