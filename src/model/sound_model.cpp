#include "sound_model.h"

SoundModel::SoundModel(QObject *parent)
    : QAbstractTableModel(parent)
    , repo_(new SoundRepository(this))
    , sounds_()
{
    init();
}

int SoundModel::columnCount(const QModelIndex &) const
{
    return 2; // (uuid/)name, local_path
}

int SoundModel::rowCount(const QModelIndex &) const
{
    return sounds_.size();
}

QVariant SoundModel::data(const QModelIndex &index, int role) const
{
    if(!indexIsValid(index))
        return QVariant();

    if(role == Qt::DisplayRole) {
        if(index.column() == 0)
            return QVariant(sounds_[index.row()].resource.name);
        else if(index.column() == 1)
            return QVariant(sounds_[index.row()].local_path);
    }
    else if(role == Qt::EditRole) {
        if(index.column() == 0)
            return QVariant(sounds_[index.row()].uuid);
        else if(index.column() == 1)
            return QVariant(sounds_[index.row()].local_path);
    }

    return QVariant();
}

QVariant SoundModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // horizontal header (attribute names)
    if(orientation == Qt::Horizontal) {
        if(section < 0 || section >= columnCount())
            return QVariant();

        if(role == Qt::DisplayRole) {
            if(section == 0)
                return QVariant("Name");
            else if(section == 1)
                return QVariant("Local Path");
        }

        else if(role == Qt::EditRole) {
            if(section == 0)
                return QVariant("UUID");
            else if(section == 1)
                return QVariant("Path");
        }
    }

    // vertical header (number of record)
    else {
        if(section < 0 || section >= rowCount())
            return QVariant();
        if(role == Qt::DisplayRole || role == Qt::EditRole)
            return QVariant(section);
    }

    return QVariant();
}

Qt::ItemFlags SoundModel::flags(const QModelIndex &index) const
{
    if(!indexIsValid(index))
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}

bool SoundModel::setData(const QModelIndex &, const QVariant &, int)
{
    // TODO support edit
    return false;
}

bool SoundModel::setHeaderData(int, Qt::Orientation, const QVariant &, int)
{
    return false;
}

/*bool SoundModel::removeColumns(int, int, const QModelIndex&)
{
    return false;
}

bool SoundModel::removeRows(int, int, const QModelIndex&)
{
    // TODO support remove
    return false;
}*/

void SoundModel::update()
{
    repo_->getSounds();
}

void SoundModel::onRepoReceivedSounds(const QList<Sound> &sounds)
{
    sounds_ = sounds;

    emit layoutAboutToBeChanged();
    emit layoutChanged();
    emit dataChanged(index(0,0), index(rowCount(), columnCount()));
}

bool SoundModel::indexIsValid(const QModelIndex &index) const
{
    return index.isValid() && index.row() < rowCount() && index.column() < columnCount();
}

void SoundModel::init()
{
    connect(repo_, &SoundRepository::soundsReceived,
            this, &SoundModel::onRepoReceivedSounds);

    update();
}
