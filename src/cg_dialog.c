#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "cg_dialog.h"
#include "cg_constants.h"

int width, height, fullscreen = 1, quit;

/* ******
 * Events
 * ****** */

static gboolean delete_event(){
   quit = CG_QUIT;
   return FALSE;
}

static void destroy_event(){
   quit = CG_QUIT;
   gtk_main_quit();
}

static void start_event(GtkWidget *widget, gpointer data){
   char *widthstr, *heightstr;
   char *text;

   if(widget == NULL) return;
   quit       = CG_RUNDEMO;
   text       = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data));
   widthstr   = strtok((char*)text, "x");
   heightstr  = strtok(NULL, "\0");
   width      = atoi(widthstr);
   height     = atoi(heightstr);
   gtk_main_quit();
}

static void toggle_event(GtkWidget *widget){
   if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))){
      fullscreen = 1;
   }
   else{
      fullscreen = 0;
   }
}

/* **********************
 * Dialog creation method
 * ********************** */

void createWindow(){
   GtkWidget *window,
             *startbutton,
             *exitbutton,
             *togglefs,
             *hbox,
             *vbox,
             *resolutions;

   /* Widget initialization */
   gtk_init(NULL, NULL);
   window      = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   startbutton = gtk_button_new_with_label("Demo");
   exitbutton  = gtk_button_new_with_label("Die");
   togglefs    = gtk_check_button_new_with_label("Fullscreen");
   hbox        = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
   vbox        = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
   resolutions = gtk_combo_box_text_new();

   /* Events */
   g_signal_connect(window,      "delete-event", G_CALLBACK(delete_event),  NULL);
   g_signal_connect(window,      "destroy",      G_CALLBACK(destroy_event), NULL);
   g_signal_connect(exitbutton,  "clicked",      G_CALLBACK(destroy_event), NULL);
   g_signal_connect(startbutton, "clicked",      G_CALLBACK(start_event),   (gpointer)resolutions);
   g_signal_connect(togglefs,    "toggled",      G_CALLBACK(toggle_event),  NULL);

   /* Set properties of the widgets */
   gtk_window_set_title(GTK_WINDOW(window), "Cybernetic genetics");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);
   gtk_window_set_default_size(GTK_WINDOW(window), 330,100);
   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolutions), "640x480");
   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolutions), "800x600");
   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolutions), "1024x768");
   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolutions), "1366x768");
   gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(resolutions), "1280x1024");
   gtk_combo_box_set_active(GTK_COMBO_BOX(resolutions), 2);
   gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(togglefs), TRUE);

   /* Build layout */
   gtk_container_add(GTK_CONTAINER(hbox), startbutton);
   gtk_container_add(GTK_CONTAINER(hbox), exitbutton);
   gtk_box_pack_start(GTK_BOX(vbox), resolutions, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), togglefs, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   /* See widgets */
   gtk_widget_show(startbutton);
   gtk_widget_show(exitbutton);
   gtk_widget_show(togglefs);
   gtk_widget_show(resolutions);
   gtk_widget_show(hbox);
   gtk_widget_show(vbox);
   gtk_widget_show(window);
}

/*
 * cgDialog is the connection between the application and the
 * Window content managment system.
 */
int cgDialog(int *w, int *h, int *f){

   createWindow();
   gtk_main();

   if(quit == CG_QUIT){
      return(CG_QUIT);
   }

   *w = width;
   *h = height;
   *f = fullscreen;

   return(CG_RUNDEMO);
}
