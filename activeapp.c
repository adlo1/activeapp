#include <gtk/gtk.h>
#include <libwnck/libwnck.h>
#include <string.h>
#define BUF_LENGTH 36
#define MAX_WIDTH_CHARS 30

typedef struct infowidget {
  GtkWidget *label;
  GtkWidget *icon;
  GdkPixbuf *pixbuf;
  GtkWidget *event_box;
  GtkWidget *menu;
  GtkWidget *action_menu;
  
}infowidget;

static gint
popup_handler (GtkWidget *widget, GdkEvent *event)
{
	GdkEventButton *event_button = (GdkEventButton *) event;
	
	gtk_menu_popup (GTK_MENU (widget), NULL, NULL, NULL, NULL,
					event_button->button, event_button->time);
				
}
static void
on_active_window_changed (WnckScreen *screen, WnckWindow *previous_window,  gpointer data)

	{gchar name[BUF_LENGTH];
		infowidget *inwidget = (infowidget*)data;
		WnckWindow *active_window;
		active_window = wnck_screen_get_active_window(screen);
		WnckWindowType type;
		WnckApplication *app;
		
		gtk_image_clear(GTK_IMAGE(inwidget->icon));
		
		if (active_window)
		{
			type=wnck_window_get_window_type (active_window);
			if (type == WNCK_WINDOW_DESKTOP || type == WNCK_WINDOW_DOCK || type == WNCK_WINDOW_UTILITY)
			{	
				gtk_label_set_text(GTK_LABEL(inwidget->label),"");
			}
			
			else
			{
				/* 
				 * 
				 * if (inwidget->action_menu)
					gtk_widget_destroy (GTK_WIDGET(inwidget->action_menu));
					
				inwidget->action_menu = wnck_action_menu_new (active_window);
				
				g_signal_handlers_disconnect_matched (inwidget->action_menu, G_SIGNAL_MATCH_FUNC, 
					NULL, NULL, NULL, popup_handler, NULL);
					
				g_signal_connect_swapped (inwidget->event_box, "button-press-event", 
					G_CALLBACK(popup_handler), GTK_WIDGET (inwidget->action_menu));
					* 
					*/
				
				app=wnck_window_get_application(active_window);
				strncpy (name, wnck_application_get_name (app), BUF_LENGTH);
				name[BUF_LENGTH-1]='\0';
				
				if (wnck_window_is_skip_pager(active_window))
				{gtk_label_set_text(GTK_LABEL(inwidget->label),"");
				//app=wnck_window_get_application(active_window);
				//gtk_label_set_text(GTK_LABEL(data),wnck_application_get_name (app));
			}
			else
			{gtk_label_set_text(GTK_LABEL(inwidget->label),name);
				inwidget->pixbuf=wnck_window_get_icon (active_window);
				gtk_image_set_from_pixbuf(GTK_IMAGE(inwidget->icon),inwidget->pixbuf);
			}
			}
}
	else
	{gtk_label_set_text(GTK_LABEL(inwidget->label),"");
		gtk_image_clear(GTK_IMAGE(inwidget->icon));
		}
	}

int
main (int    argc,
      char **argv)
{
	
	static GtkWidget *window = NULL;
  GtkWidget *hbox;
  GtkWidget *vbox;
  
  

infowidget widget;
  
widget.action_menu = 0;
  
	 
	gtk_init (&argc, &argv);
  WnckScreen *screen;
 screen = wnck_screen_get_default ();
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (window, "destroy",
		    G_CALLBACK (gtk_main_quit),
		    NULL);

  gtk_window_set_title (GTK_WINDOW (window), "Application name");
  hbox = gtk_hbox_new (FALSE, 5);
  vbox = gtk_vbox_new (FALSE, 5);
  widget.event_box = gtk_event_box_new ();
  
  gtk_container_add (GTK_CONTAINER (window), vbox);
  gtk_container_add (GTK_CONTAINER (vbox), widget.event_box);
  gtk_container_add (GTK_CONTAINER (widget.event_box), hbox);
  
  gtk_container_set_border_width (GTK_CONTAINER (window), 5);
  
  
    
  widget.icon = gtk_image_new();
  gtk_box_pack_start (GTK_BOX (hbox), widget.icon, FALSE, FALSE, 0);
  gtk_widget_show (widget.icon);
  widget.label = gtk_label_new("");
  gtk_label_set_max_width_chars (GTK_LABEL(widget.label), MAX_WIDTH_CHARS - 2);
  gtk_label_set_ellipsize (GTK_LABEL (widget.label), PANGO_ELLIPSIZE_END);
  gtk_box_pack_start (GTK_BOX (hbox), widget.label, FALSE, FALSE, 0); 
  gtk_widget_show(widget.label);
  
  g_signal_connect (screen, "active-window-changed",
                    G_CALLBACK (on_active_window_changed), &widget);
                   
   gtk_widget_show_all(window);
   
     
                                      
  
   gtk_main ();
  return 0;
}
