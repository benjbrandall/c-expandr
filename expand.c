#include <stdio.h>
#include <time.h>
#include <gtk/gtk.h>

// build: gcc -o simple simple.c `pkg-config --libs --cflags gtk+-2.0`

char path[200] = "/home/benjamin/Documents/Snippets/";
char config_file[200] = ".expandr.conf";
char pid_file[200] = ".pid.txt";
char ext[10] = ".txt";

const char* get_window_title(GtkWidget *widget) {

  GtkWidget *toplevel = gtk_widget_get_toplevel (widget);
    if (gtk_widget_is_toplevel (toplevel))
  {
    return gtk_window_get_title(toplevel);
  }

}

void on_save_click(GtkWidget *button, gpointer data) {

  GtkTextIter start, end;

  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));

  gtk_text_buffer_get_bounds(buffer, &start, &end);
  gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

  char abbreviation[50] = "";
  char filename[100] = "";
  strcat(abbreviation,path);
  strcat(abbreviation,get_window_title(button));
  strcat(abbreviation,ext);

  printf("Snippet saved to: %s\n",abbreviation);

  g_file_set_contents (abbreviation, text, -1, NULL); // load snippet file contents into memory
  // do the saving

  puts("Saved, destroying self!");

  GtkWidget *toplevel = gtk_widget_get_toplevel (data);
    if (gtk_widget_is_toplevel (toplevel))
  {
    gtk_widget_destroy(toplevel);
  }

}

void on_new_click(GtkEntry *entry, gpointer data) {

  GtkWidget *new_window;
  GtkWidget *vbox; // with the text_entry
  GtkWidget *snippet_field;
  GtkTextBuffer *buffer;
  GtkWidget *save_button;

  const gchar *text = gtk_entry_get_text(GTK_ENTRY(data)); // get the text user entered

  buffer = gtk_text_buffer_new(NULL);
  snippet_field = gtk_text_view_new_with_buffer(buffer);
  gtk_text_view_set_editable(snippet_field, TRUE);

  new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), text);
  gtk_window_set_default_size(GTK_WINDOW(new_window), 300, 200);
  gtk_window_set_position(GTK_WINDOW(new_window), GTK_WIN_POS_CENTER_ALWAYS);
  gtk_container_set_border_width (GTK_CONTAINER (new_window), 5);

  save_button = gtk_button_new_with_label("Save");
  gtk_widget_set_size_request(save_button, 80, 40);
  vbox = gtk_vbox_new(FALSE, 1);
  gtk_box_pack_start(GTK_BOX(vbox), snippet_field, TRUE, TRUE, 3);
  gtk_box_pack_start(GTK_BOX(vbox), save_button, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(new_window), vbox);
  gtk_widget_show_all(new_window);

  g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_click), snippet_field);


}

void destroy_myself(GtkWidget *entry, GtkClipboard *clipboard) {

  gtk_clipboard_store(clipboard);

  GtkWidget *toplevel = gtk_widget_get_toplevel (entry);
    if (gtk_widget_is_toplevel (toplevel))
  {
    gtk_widget_hide(toplevel);
  }

}

int add_snippet_to_clipboard(char* contents, GtkWidget *entry) {

  GtkClipboard *clipboard;
  clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
  gtk_clipboard_set_can_store(clipboard, 0, 0);
  // Copy contents to clipboard - the program's logical end
  gtk_clipboard_set_text(clipboard, contents, -1);
  destroy_myself(entry, clipboard);
}

void get_snippet_text(char* full_path, GtkWidget *entry) {

  gchar *contents;
  GError *err = NULL;

  g_file_get_contents (full_path, &contents, NULL, &err); // load snippet file contents into memory
  g_assert ((contents == NULL && err != NULL) || (contents != NULL && err == NULL));

  if (err != NULL)
    {
      // Report error to user, and free error
      g_assert (contents == NULL);
      fprintf (stderr, "Unable to get snippet: %s\n", err->message);
      full_path = "";
      g_error_free (err);
    }
  else
    {
      // Use file contents
      g_assert (contents != NULL);
      add_snippet_to_clipboard(contents, entry); // here's where we're off to, hopefully
    }

}

void on_entry_activate(GtkEntry *entry, gpointer data) {

  const gchar *text = gtk_entry_get_text(GTK_ENTRY(data)); // get the text user entered
  char *filename;
  char *full_path;
  filename = strcat(text,ext); // turn user text into filename.txt
  full_path = strcat(path,filename); // add filename.txt to user's snippet path
  gtk_entry_set_text(GTK_ENTRY(entry), ""); // set text field back to blank
  get_snippet_text(full_path, entry); // go look for the contents of the snippet file
  full_path = "";
}

int main(int argc, char *argv[]) {

  GtkWidget *window;
  GtkWidget *entry;
  GtkWidget *box;
  GtkWidget *button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "expand!");
    gtk_container_set_border_width (GTK_CONTAINER (window), 5);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);


    button = gtk_button_new_with_label("New");

    entry = malloc(sizeof(GtkWidget));
    entry = gtk_entry_new();

    box = gtk_hbox_new(FALSE, 0);

    gtk_box_pack_start (GTK_BOX(box), entry, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX(box), button, TRUE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);

    g_signal_connect(entry, "activate", G_CALLBACK(on_entry_activate), entry);
    g_signal_connect(button, "activate", G_CALLBACK(on_new_click), entry);

    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    return 0;

}
