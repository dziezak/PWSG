using Potter.API.Interfaces;
using Potter.API.Models;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows.Input;

namespace MiNIPotter;

public sealed class BooksViewModel : INotifyPropertyChanged
{
	private readonly IBooksRepository _booksRepository;

	public BooksViewModel(IBooksRepository booksRepository)
	{
		_booksRepository = booksRepository;
		LoadBooksCommand = new RelayCommand(LoadBooksAsync, () => !IsLoading);
	}

	public ICommand LoadBooksCommand { get; }

	private ObservableCollection<Book> _books = [];

	public ObservableCollection<Book> Books
	{
		get => _books;
		set
		{
			if (_books != value)
			{
				_books = value;
				OnPropertyChanged();
			}
		}
	}

	private bool _isLoading;

	public bool IsLoading
	{
		get => _isLoading;
		private set
		{
			if (_isLoading != value)
			{
				_isLoading = value;
				OnPropertyChanged();
				CommandManager.InvalidateRequerySuggested();
			}
		}
	}

	public async Task LoadBooksAsync()
	{
		if (IsLoading) return;

		try
		{
			IsLoading = true;
			var books = await _booksRepository.GetBooksAsync();

			Books.Clear();
			foreach (var book in books)
				Books.Add(book);
		}
		finally
		{
			IsLoading = false;
		}
	}

	public event PropertyChangedEventHandler? PropertyChanged;
	private void OnPropertyChanged([CallerMemberName] string? propName = null)
		=> PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propName));
}

public class RelayCommand : ICommand
{
	public event EventHandler? CanExecuteChanged
	{
		add => CommandManager.RequerySuggested += value;
		remove => CommandManager.RequerySuggested -= value;
	}

	private readonly Func<Task> _execute;
	private readonly Func<bool>? _canExecute;

	public RelayCommand(Func<Task> execute, Func<bool>? canExecute = null)
	{
		_execute = execute;
		_canExecute = canExecute;
	}

	public bool CanExecute(object? parameter) => _canExecute?.Invoke() ?? true;
	public async void Execute(object? parameter) => await _execute();
}