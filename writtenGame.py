"""Analyze a small set of numbers and explain the mathematics involved."""

from math import isfinite, isqrt
from statistics import median


def is_prime(number):
	"""Return True if number is a prime integer."""
	if number < 2:
		return False
	for divisor in range(2, isqrt(number) + 1):
		if number % divisor == 0:
			return False
	return True


def analyze(numbers):
	"""Display descriptive statistics and prime values for the data."""
	if not numbers:
		print("No numbers to analyze.")
		return

	total = sum(numbers)
	average = total / len(numbers)
	smallest = min(numbers)
	largest = max(numbers)

	print("\nAnalysis")
	print(f"Data: {numbers}")
	print(f"Count: {len(numbers)} (how many values are in the data set)")
	print(f"Sum: {total:g} (the values added together)")
	print(f"Mean: {average:g} (sum divided by count)")
	print(f"Median: {median(numbers):g} (the middle value after sorting)")
	print(f"Minimum: {smallest:g}; maximum: {largest:g}")
	print(f"Range: {largest - smallest:g} (maximum minus minimum)")

	primes = [int(value) for value in numbers
			  if value.is_integer() and is_prime(int(value))]
	if primes:
		print(f"Prime numbers: {primes} (each is greater than 1 and has only 1 and itself as factors)")
	else:
		print("Prime numbers: none (a prime is a whole number greater than 1 with exactly two factors)")


def main():
	entered = input(
		"Enter numbers separated by commas, or press Enter for sample data: "
	).strip()

	if not entered:
		numbers = [2.0, 3.0, 4.0, 7.0, 10.0]
		print("Using sample data: 2, 3, 4, 7, 10")
	else:
		try:
			numbers = [float(part.strip()) for part in entered.split(",")]
		except ValueError:
			print("Invalid input. Enter numbers separated by commas, such as 2, 3, 4.5.")
			return
		if not all(isfinite(value) for value in numbers):
			print("Please enter finite numbers only.")
			return

	analyze(numbers)


if __name__ == "__main__":
	main()
