using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace ChatApp
{
    public partial class MainWindow : Window
    {
        private bool isConnected = false;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Group Chat App v1.0\nCreated with WPF", "About", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        private void MessageTextBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                if (Keyboard.Modifiers.HasFlag(ModifierKeys.Shift))
                {
                    return;
                }

                e.Handled = true;
                SendMessage();
            }
        }

        private void SendMessage()
        {
            string text = MessageTextBox.Text.Trim();
            if (string.IsNullOrEmpty(text)) return;

            // Tworzenie zaokrąglonego okienka z wiadomością
            Border messageBubble = new Border
            {
                Background = new SolidColorBrush(Colors.Orange),
                CornerRadius = new CornerRadius(10),
                Padding = new Thickness(10),
                Margin = new Thickness(5)
            };

            TextBlock messageText = new TextBlock
            {
                Text = $"User: {text}",
                Foreground = new SolidColorBrush(Colors.White)
            };

            messageBubble.Child = messageText;
            MessagesPanel.Children.Add(messageBubble);

            MessageTextBox.Clear();
            MessagesScrollViewer.ScrollToEnd();
        }

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
