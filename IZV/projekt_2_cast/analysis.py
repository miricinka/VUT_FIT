#!/usr/bin/env python3.8
# coding=utf-8
__author__ = "Mirka Kolarikova"

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import os

# Task 1: load data
def get_dataframe(filename: str, verbose: bool = False) -> pd.DataFrame:

    # load pickle dataset
    data = pd.read_pickle(filename, compression='gzip')

    # count and print original size
    if verbose:
        orig_size = data.memory_usage(index=False, deep=True).sum()
        print("orig_size={:.1f} MB".format(orig_size/1048576))

    # categorize some collumns
    cols = ['p36', 'weekday(p2a)', 'p6', 'p7', 'p8', 'p9', 'p10', 'p11',
            'p12', 'p13a', 'p13b', 'p13c', 'p15', 'p16', 'p17', 'p18',
            'p19', 'p20', 'p21', 'p22', 'p23', 'p24', 'p27', 'p28',
            'p34', 'p35', 'p39', 'p44', 'p45a', 'p47', 'p48a', 'p49',
            'p50a', 'p50b', 'p51', 'p52', 'p53', 'p55a', 'p57', 'p58',
            'p5a', 'h', 'j', 'p', 'q', 't']
    data[cols] = data[cols].astype('category')

    # rename p2a to date and change type to datetime
    data['p2a'] = data['p2a'].astype('datetime64')
    data.rename(columns={'p2a': 'date'}, inplace=True)

    # count and print new size
    if verbose:
        new_size = data.memory_usage(index=False, deep=True).sum()
        print("new_size={:.1f} MB".format(new_size/1048576))
    # return dataframe
    return data


# Task 2: consequences of accidents in individual regions
def plot_conseq(df: pd.DataFrame, fig_location: str = None,
                show_figure: bool = False):

    # style graphs
    sns.set_style("darkgrid")
    fig, axes = plt.subplots(nrows=4, ncols=1, figsize=(8.27, 8.27))
    plt.rcParams['figure.dpi'] = 360
    plt.rcParams['text.color'] = '#909090'
    plt.rcParams['font.sans-serif'] = 'Arial'
    plt.rcParams['font.family'] = 'sans-serif'
    sns.despine()

    # convert to integers
    df.p13a = df.p13a.astype(int)
    df.p13b = df.p13b.astype(int)
    df.p13c = df.p13c.astype(int)

    # prepare variable items 
    columns = ['p13a', 'p13b', 'p13c', 'p1']
    color = ['red', 'orange', 'gold', 'black']
    title = ['Úmrtí', 'Těžce ranění', 'Lehce ranění', 'Celkový počet nehod']

    data = (df.groupby(['region']).agg({'p13a': 'sum', 'p13b': 'sum',
                                       'p13c': 'sum', 'p1': 'count'})
                                  .reset_index())

    sort = data.sort_values(by=['p1'], ascending=False)['region']

    # plot 4 graphs
    for i in range(4):
        sns.barplot(ax=axes[i],
                    data=data,
                    x='region',
                    y=columns[i],
                    saturation=1,
                    order=sort,
                    color=color[i])
        axes[i].set_title(title[i], fontweight="bold")
        axes[i].set(ylabel='', xlabel='')
        axes[i].spines['bottom'].set_color('#404040')

    plt.tight_layout()

    # save or show
    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        # create folder if doesnt exist
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()

# Task 3: cause of accident and damage
def plot_damage(df: pd.DataFrame, fig_location: str = None,
                show_figure: bool = False):

    sns.set_style("darkgrid")

    bins = pd.IntervalIndex.from_tuples([(100, 100), (201, 209),
                                         (301, 311), (401, 414),
                                         (501, 516), (601, 615)],
                                        closed='both')

    df.set_index('region', inplace=True)
    city = df.loc[["PHA", "JHM", "OLK", "KVK"], ['p53', 'p12']]
    city['p53'] = city['p53'].astype(int).div(10)

    # prepare data
    df_bins = pd.cut(x=city['p12'].to_list(), bins=bins)
    df_bins.categories = ['Nezaviněna řidičem', 'nepřiměřená rychlost jízdy',
                          'nesprávné předjíždění', 'nedání přednosti v jízdě',
                          'nesprávný způsob jízdy', 'technická závada vozidla']
    city['p12'] = df_bins
    city['loss'] = pd.cut(x=city['p53'],
                          bins=[-1, 50, 200, 500, 1000, float('inf')],
                          right=True,
                          labels=['0-50', '51-200', '201-500',
                                  '501-1000', '>1000'])
    city.reset_index(inplace=True)
    city = (city.groupby(['region', 'p12', 'loss'])['p53'].agg('count')
                                                          .reset_index())

    # plot bar graph with 4 subplots
    g = sns.catplot(x="loss", y='p53', col="region", col_wrap=2,
                    hue='p12', data=city, palette='hls', legend=False,
                    col_order=["PHA", "JHM", "OLK", "KVK"],
                    kind="bar", height=4.2, aspect=1, saturation=1)

    # style graph
    g.set(yscale="log").set_titles("{col_name}", fontweight="bold",
                                   color='#404040', size=15)
    g.set_axis_labels("Škoda v tisících Kč", "Počet nehod",
                      fontweight="bold", color='#404040')

    # style axes
    for axe in g.axes:
        axe.spines['bottom'].set_color('#404040')
        axe.spines['left'].set_color('#404040')

    # loc legend
    plt.legend(loc='upper right', title='Příčina nehody')

    # save or show
    plt.tight_layout()
    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        # create folder if doesnt exist
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()

# Task 4: road surface
def plot_surface(df: pd.DataFrame, fig_location: str = None,
                 show_figure: bool = False):

    # select data
    data = df[['region', 'date', 'p16']].set_index('region')
    data = data.loc[["JHC", "HKK", "OLK", "KVK"]].reset_index()

    # prepare data
    table = pd.crosstab(index=[data['region'], data['date']],
                        columns=data['p16'])
    table.rename(columns={0: 'jiný stav', 1: 'suchý - neznečištěný',
                          2: 'suchý - znečištěný', 3: 'mokrý',
                          4: 'bláto', 5: 'náledí, sníh - posypané',
                          6: 'náledí, sníh - neposypané',
                          7: 'rozlitý olej, nafta apod.',
                          8: 'souvislá sněhová vrstva, rozbředlý sníh',
                          9: 'náhlá změna stavu'},
                 inplace=True)
    table = table.reset_index().set_index('date')
    table = table.groupby(['region']).resample("M").sum()
    table = table.stack().reset_index().set_index('region')
    table.rename(columns={0: 'Počet nehod', 'p16': 'Stav vozovky'},
                 inplace=True)

    # set graph style
    sns.set_style("darkgrid")

    # plot line graph with 4 subplots
    g = sns.relplot(x='date', y='Počet nehod', col='region', col_wrap=1,
                    hue='Stav vozovky', data=table, palette='hls',
                    col_order=["JHC", "HKK", "OLK", "KVK"],
                    height=2, aspect=4, kind='line')

    # style titles and legend
    g.set_titles("{col_name}", fontweight="bold", color='#404040',
                 size=15)
    g.set_axis_labels("Datum vzniku nehody", "Počet nehod",
                      fontweight="bold", color='#404040')
    g.tight_layout()
    plt.setp(g._legend.get_title(), fontsize=15, color='#404040',
             fontweight="bold")

    # save or show
    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        # create folder if doesnt exist
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    df = get_dataframe("accidents.pkl.gz", verbose=False)
    # plot_conseq(df, fig_location="grafy/01_nasledky.pdf", show_figure=True)
    # plot_damage(df, "grafy/02_priciny.pdf", True)
    plot_surface(df, "grafy/03_stav.pdf", True)
