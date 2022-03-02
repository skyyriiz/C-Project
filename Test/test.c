#include <gtk/gtk.h> // Biblio
#include <stdio.h>

GtkWidget *windows; // Variable pour la fenetre
GtkLabel *label; // Variable pour le label

int main(int argc, char *argv[]) {
    GtkBuilder *builder; // Pour le build
    gtk_init(&argc, &argv); // Lancer gtk

    builder = gtk_builder_new(); // Crée le build
    gtk_builder_add_from_file(builder, "test_glade.glade", NULL); // Charger le fichier

    windows = GTK_WIDGET(gtk_builder_get_object(builder, "windows")); // Charger la fenetre
    label = GTK_LABEL(gtk_builder_get_object(builder, "label")); // Charger le label

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
void button_clicked(){
    printf("Button clicked \n");
}