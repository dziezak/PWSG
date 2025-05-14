using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media.Imaging;
using Potter.API;
using Potter.API.Interfaces;

namespace MiNIPotter
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly IBooksRepository _booksRepository = new BooksRepository();
        public BookDisplay SelectedBook { get; set; }

        public MainWindow()
        {
            InitializeComponent();
        }

        public async void LoadData_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // Pobranie książek asynchronicznie
                var books = await _booksRepository.GetBooksAsync();

                // Konwersja danych do formatu obsługiwanego przez ListBox
                var bookList = books.Select(book => new BookDisplay
                {
                    Id = book.Id,
                    Title = book.Title,
                    Author = book.Author,
                    Dedication = book.Dedication,
                    Summary = book.Summary,
                    PageCount = book.Pages,
                    ReleaseDate = book.ReleaseDate.ToString("d MMMM yyyy"),
                    CoverImage = CreateBitmapImage(book.Cover),
                    AverageRating = book.Chapters.Any() ? book.Chapters.Average(chapter => chapter.Rating) : 0
                }).ToList();

                // Przypisanie danych do ListBoxBooks
                ListBoxBooks.ItemsSource = bookList;
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Błąd podczas ładowania danych: {ex.Message}", "Błąd", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        public void BookSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (ListBoxBooks.SelectedItem is BookDisplay book)
            {
                SelectedBook = book;
                DataContext = SelectedBook;
            }
        }

        private BitmapImage CreateBitmapImage(string uri)
        {
            try
            {
                if (!Uri.IsWellFormedUriString(uri, UriKind.Absolute))
                    throw new UriFormatException("Niepoprawny format URI");

                return new BitmapImage(new Uri(uri, UriKind.Absolute));
            }
            catch
            {
                return new BitmapImage(new Uri("pack://application:,,,/Images/default_cover.png")); // Ustaw domyślną okładkę w razie błędu
            }
        }
    }

    // Pomocnicza klasa do wyświetlania danych w ListBox i szczegółach powieści
    public class BookDisplay
    {
        public Guid Id { get; set; }
        public string Title { get; set; }
        public string Author { get; set; }
        public string Dedication { get; set; }
        public string Summary { get; set; }
        public int PageCount { get; set; }
        public string ReleaseDate { get; set; }
        public BitmapImage CoverImage { get; set; }
        public double AverageRating { get; set; }
        public List<Image> StarRating => Enumerable.Repeat(
            new Image { Source = new BitmapImage(new Uri("pack://application:,,,/Images/star.png")) },
            (int)Math.Clamp(AverageRating, 0, 10)).ToList();
    }
}
