#include <gtk/gtk.h> // Biblio
#include <stdio.h>
#include <curl

GtkWidget *windows; // Variable pour la fenetre
GtkWidget *windows_settings; // Variable pour la fenetre
GtkWidget *windows_game;
GtkWidget *windows_result;

int main(int argc, char *argv[]) {
    GtkBuilder *builder; // Pour le build
    gtk_init(&argc, &argv); // Lancer gtk

    builder = gtk_builder_new(); // Crée le build
    gtk_builder_add_from_file(builder, "start.glade", NULL); // Charger le fichier

    windows = GTK_WIDGET(gtk_builder_get_object(builder, "start")); // Charger la fenetre

    // Nécessaire pour un code qui utilise gtk
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(windows); // Afficher notre fenetre
    gtk_main(); // Lunch

    return 0;
}
// Pour quitter l'app
void exit_app(){
    printf("Quiiter \n");
    gtk_main_quit(); // commande pour quitter
}

// Reaction du button
void button_settings(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "settings.glade", NULL); // Charger le fichier

    windows_settings = GTK_WIDGET(gtk_builder_get_object(builder, "windows_settings"));
    gtk_window_set_default_size(GTK_WINDOW(windows_settings), 800, 400);
    g_signal_connect(windows_settings, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_settings);
}

void destroy_settings(){
    gtk_widget_destroy(windows_settings);
}

void button_game(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "game.glade", NULL); // Charger le fichier

    windows_game = GTK_WIDGET(gtk_builder_get_object(builder, "windows_game"));
    gtk_window_set_default_size(GTK_WINDOW(windows_game), 1920, 1080);
    g_signal_connect(windows_game, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_game);
}

void button_result(){
    GtkBuilder *builder;
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "result.glade", NULL); // Charger le fichier

    windows_result = GTK_WIDGET(gtk_builder_get_object(builder, "windows_result"));
    gtk_window_set_default_size(GTK_WINDOW(windows_result), 1920, 1080);
    g_signal_connect(windows_result, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show_all(windows_result);
}

