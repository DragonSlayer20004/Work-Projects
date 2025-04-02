"""
=============================================================================
Title
: assignment8.py
Description : This is assignment 8
Author
: Kyle Hinkle
Date: 003/24/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""
# imports
import statistics

# Given information.
sales_data = {
    'Laptop': 1500,
    'Headphones': 500,
    'Smartphone': 1200,
    'Tablet': 800,
    'Monitor': 600,
    'Keyboard': 300,
    'Mouse': 150,
    'Printer': 400,
    'Camera': 900,
    'Speakers': 350
}

calculated_sales_data = {
    'Total Sales': 0,
    'Average Sales': 0,
    'Median Sales': 0,
    'Standard Deviation of Sales': 0,
    'Highest Sales': 0,
    'Lowest Sales': 0
}

def sortProductsBySales(sales_data):
    dummy_sales_data = sales_data.copy()

    newSalesData = {}
    # Continue until all products are moved to newSalesData
    while len(dummy_sales_data) > 0:
        # Find the product with the highest sales
        highest = max(dummy_sales_data, key=dummy_sales_data.get)

        # Add the highest product to the new list
        newSalesData[highest] = dummy_sales_data[highest]

        # Remove the highest product from the original list
        del dummy_sales_data[highest]

    return newSalesData

def getSalesValue(item):
    return item[1]

# Statistics library is very useful in getting statistics. Who would have guessed.
def calculateSalesStatistics(sales_data):
    calculated_sales_data = {
    'Total Sales': 0,
    'Average Sales': 0,
    'Median Sales': 0,
    'Standard Deviation of Sales': 0,
    'Highest Sales': 0,
    'Lowest Sales': 0
    }

    # Get total sales
    total_sales = sum(sales_data.values())
    calculated_sales_data["Total Sales"] = total_sales

    # Calculate average sales
    calculated_sales_data["Average Sales"] = total_sales / len(sales_data)

    # Calculate median sales
    calculated_sales_data["Median Sales"] = statistics.median(sales_data.values())

    # Calculate standard deviation of sales
    calculated_sales_data["Standard Deviation of Sales"] = statistics.stdev(sales_data.values())

    # Find highest sales
    calculated_sales_data["Highest Sales"] = max(sales_data.values())

    # Find lowest sales
    calculated_sales_data["Lowest Sales"] = min(sales_data.values())

    return calculated_sales_data

def displaySortedSales(sortedSales, calculatedSales):
    # This for loop is a little awkward for me since I'm not good with dictionaries and prefer lists due to indexing.
    # Grab product (first variable) and sales (second variable) from the view object returned by .items().
    # This allows us to iterate through the dictionary and print everything we need.
    # The alternative is to print everything one by one using key names, but that is tedious and boring.

    print("Sorted Sales Data (Product: Sales):")
    for product, sales in sortedSales.items():
        # f print allows us to imbed variables directly into the print statement.
        print(f"{product}: ${sales:.2f}")
    
    print("\nCalculated Sales Statistics (Product: Sales):")
    for product, sales in calculatedSales.items():
        # :.2f forces it to have 2 decimal places.
        print(f"{product}: ${sales:.2f}")

    print("\nPercentage Contribution to Total Sales:")
    for product, sales in sortedSales.items():
        # Calculate the percentage contribution relative to total sales.
        percentage = (sales / calculatedSales.get("Total Sales")) * 100
        print(f"{product}: {percentage:.2f}%")

    return 0

sortedSales = sortProductsBySales(sales_data)
calculatedSales = calculateSalesStatistics(sales_data)
displaySortedSales(sortedSales, calculatedSales)