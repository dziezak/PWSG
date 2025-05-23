using System;
using System.Collections.ObjectModel;
using System.Timers;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;
using WPF2;

namespace ChatApp
{
    public partial class MainWindow : Window
    {
        private bool isConnected = false;
        private DispatcherTimer timer;
        private int messageCounter = 0;

        public ObservableCollection<Message> Messages { get; set; }

        public MainWindow()
        {
            InitializeComponent();
            Messages = new ObservableCollection<Message>();
            DataContext = this;

            timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromSeconds(30);
            timer.Tick += (s, e) => MessagesListBox.Items.Refresh();
            timer.Start();
        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            if (!isConnected)
            {
                isConnected = true;
                Messages.Add(new Message { IsSystem = true, Text = "Connected", Timestamp = DateTime.Now });
            }
        }

        private void Disconnect_Click(object sender, RoutedEventArgs e)
        {
            if (isConnected)
            {
                isConnected = false;
                Messages.Add(new Message { IsSystem = true, Text = "Disconnected", Timestamp = DateTime.Now });
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void About_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("ChatApp WPF - wersja laboratoryjna", "About", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            SendMessage();
        }

        private void MessageTextBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter && !Keyboard.IsKeyDown(Key.LeftShift))
            {
                e.Handled = true;
                SendMessage();
            }
        }

        private void SendMessage()
        {
            var text = MessageTextBox.Text.Trim();
            if (string.IsNullOrWhiteSpace(text)) return;

            Messages.Add(new Message
            {
                Username = messageCounter % 2 == 0 ? "User1" : "User2",
                Text = text,
                Timestamp = DateTime.Now,
                IsSystem = false
            });

            messageCounter++;
            MessageTextBox.Clear();
        }
    }
}
