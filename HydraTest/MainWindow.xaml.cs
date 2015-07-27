﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Linq;
using System.Windows;
using System.Windows.Media;

namespace HydraTest
{
    public partial class MainWindow
    {
		HydraLibrary _library;
		IDisposable _timerSubscription;

		public MainWindow()
        {
            InitializeComponent();
        }

		void button_Click(object sender, RoutedEventArgs e)
		{
			if (_library == null)
			{
				_library = new HydraLibrary();

				_timerSubscription = Observable.Timer(TimeSpan.Zero, TimeSpan.FromMilliseconds(33)).ObserveOnDispatcher().Subscribe(Tick);
            }
			else
			{
				_timerSubscription.Dispose();
				_library.Dispose();
				_library = null;
			}
        }

		void Tick(long time)
		{
			rectLeftButtonPressed.Fill = _library.LeftController.ButtonJustPressed() ? Brushes.DarkGray : Brushes.Transparent;
			rectLeftTriggerPressed.Fill = _library.LeftController.TriggerJustPressed() ? Brushes.DarkGray : Brushes.Transparent;
			rectRightButtonPressed.Fill = _library.RightController.ButtonJustPressed() ? Brushes.DarkGray : Brushes.Transparent;
			rectRightTriggerPressed.Fill = _library.RightController.TriggerJustPressed() ? Brushes.DarkGray : Brushes.Transparent;
        }
	}
}
