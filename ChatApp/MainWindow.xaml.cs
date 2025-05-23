using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace ChatApp
{
    public partial class MainWindow : Window
    {
        private bool isConnected = false;
        private int messageIndex = 0;

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

            // Tworzenie dymka wiadomości
            Border messageBubble = new Border
            {
                Background = new SolidColorBrush(Colors.DarkOrange),
                CornerRadius = new CornerRadius(10),
                Padding = new Thickness(10),
                Margin = new Thickness(5),
                HorizontalAlignment = messageIndex % 2 == 0 ? HorizontalAlignment.Left : HorizontalAlignment.Right
            };

            StackPanel messageContent = new StackPanel();

            // Nazwa użytkownika
            TextBlock userNameBlock = new TextBlock
            {
                Text = "User",
                Foreground = new SolidColorBrush(Colors.White),
                FontWeight = FontWeights.Bold
            };

            // Treść wiadomości
            TextBlock messageText = new TextBlock
            {
                Text = text,
                Foreground = new SolidColorBrush(Colors.White),
                TextWrapping = TextWrapping.Wrap,
                IsHitTestVisible = true
            };

            // Znacznik czasu
            DateTime now = DateTime.Now;
            TextBlock timestampBlock = new TextBlock
            {
                Text = GetTimestamp(now),
                Foreground = new SolidColorBrush(Colors.LightGray),
                FontSize = 10,
                HorizontalAlignment = HorizontalAlignment.Right,
                ToolTip = now.ToString("dd/MM/yyyy HH:mm:ss")
            };

            messageContent.Children.Add(userNameBlock);
            messageContent.Children.Add(messageText);
            messageContent.Children.Add(timestampBlock);
            messageBubble.Child = messageContent;

            MessagesPanel.Children.Add(messageBubble);
            messageIndex++;

            MessageTextBox.Clear();
            MessagesScrollViewer.ScrollToEnd();
        }

        private string GetTimestamp(DateTime time)
        {
            TimeSpan diff = DateTime.Now - time;

            if (diff.TotalMinutes < 1) return "Now";
            if (diff.TotalMinutes < 15) return $"{(int)diff.TotalMinutes}m ago";
            if (diff.TotalHours < 24) return time.ToString("HH:mm");
            return time.ToString("dd/MM/yyyy");
        }

        private void ConnectMenuItem_Click(object sender, RoutedEventArgs e)
        {
            isConnected = true;
            UpdateMenuItems();
            AddSystemMessage("Connected");
        }

        private void DisconnectMenuItem_Click(object sender, RoutedEventArgs e)
        {
            isConnected = false;
            UpdateMenuItems();
            AddSystemMessage("Disconnected");
        }

        private void UpdateMenuItems()
        {
            ConnectMenuItem.IsEnabled = !isConnected;
            DisconnectMenuItem.IsEnabled = isConnected;
        }

        private void AddSystemMessage(string message)
        {
            TextBlock systemMessage = new TextBlock
            {
                Text = message,
                Foreground = new SolidColorBrush(Colors.DarkGray),
                FontStyle = FontStyles.Italic,
                FontSize = 12,
                HorizontalAlignment = HorizontalAlignment.Center
            };

            MessagesPanel.Children.Add(systemMessage);
        }
    }
}
