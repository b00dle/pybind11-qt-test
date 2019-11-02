#ifndef SOUNDMODEL_H
#define SOUNDMODEL_H

#include <QAbstractTableModel>

#include "repository/sound_repository.h"
#include "http_types.h"

class SoundModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SoundModel(QObject *parent = nullptr);
    ~SoundModel() = default;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::DisplayRole);

    //bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());

    //bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /* update model data from repository */
    void update();

Q_SIGNALS:

public Q_SLOTS:

private Q_SLOTS:
    void onRepoReceivedSounds(const QList<Sound>& sounds);

private:
    /* validates existance of given QModelIndex for this model **/
    bool indexIsValid(const QModelIndex&) const;

    void init();

    SoundRepository* repo_;
    QList<Sound> sounds_;
};

#endif // SOUNDMODEL_H
