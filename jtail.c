#include <systemd/sd-journal.h>
#include <stdio.h>
#include <string.h>
#include <iv.h>

#include "utils.h"

#define LAST_MESSAGE_NUMBER 10

struct iv_fd journal_fd;

typedef struct _JournalPrivateData
{
  sd_journal* journal;
} JournalPrivateData;


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


void
process_available_messages_callback(void *data)
{
  JournalPrivateData* priv_data = (JournalPrivateData*) data;
  sd_journal* journal = priv_data->journal;

  sd_journal_process(journal);
  process_available_messages(journal);
}


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
  JournalPrivateData priv_data;

  iv_init();
  IV_FD_INIT(&journal_fd);

  ret = sd_journal_open(&journal, SD_JOURNAL_LOCAL_ONLY);

  ret = sd_journal_seek_tail(journal);
  ret = print_err("Failed to seek to the tail of the journal: %s\n", ret);

  if (ret)
    return 1;

  seek_back_by_n(journal, LAST_MESSAGE_NUMBER);
  /* process the last LAST_MESSAGE_NUMBER messages */
  process_available_messages(journal);

  priv_data.journal = journal;

  journal_fd.fd = sd_journal_get_fd(journal);
  journal_fd.handler_in = process_available_messages_callback;
  journal_fd.cookie = &priv_data;
  iv_fd_register(&journal_fd);

  iv_main();

  iv_deinit();
  sd_journal_close(journal);
  return 0;
}
