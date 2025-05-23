using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace ChatApp
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private bool isConnected = false;

        public MainWindow()
        {
            InitializeComponent();
        }

        // Obsługa przycisku Exit
        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        // Obsługa przycisku About
        private void About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Group Chat App v1.0\nCreated with WPF", "About", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        // Obsługa przycisku Send
        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        // Obsługa klawisza Enter i Shift+Enter
        private void MessageTextBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                if (Keyboard.Modifiers.HasFlag(ModifierKeys.Shift))
                {
                    // Shift + Enter => nowa linia
                    return;
                }

                // Enter => wyślij wiadomość
                e.Handled = true;
                SendMessage();
            }
        }

        private void SendMessage()
        {
            string text = MessageTextBox.Text.Trim();
            if (string.IsNullOrEmpty(text)) return;

            MessagesListBox.Items.Add($"User: {text}");

            MessageTextBox.Clear();
            MessagesListBox.ScrollIntoView(MessagesListBox.Items[MessagesListBox.Items.Count - 1]);
        }

        // Przełącznik połączenia
        private void ConnectMenuItem_Click(object sender, RoutedEventArgs e)
        {
            isConnected = true;
            UpdateMenuItems();
        }

        private void DisconnectMenuItem_Click(object sender, RoutedEventArgs e)
        {
            isConnected = false;
            UpdateMenuItems();
        }

        private void UpdateMenuItems()
        {
            ConnectMenuItem.IsEnabled = !isConnected;
            DisconnectMenuItem.IsEnabled = isConnected;
        }
    }
}
