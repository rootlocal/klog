/***************************************************************************
                          searchmodel.cpp  -  description
                             -------------------
    begin                : sep 2020
    copyright            : (C) 2020 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "searchmodel.h"

SearchModel::SearchModel(Awards *awards, QObject *parent):
    QSqlRelationalTableModel(parent),
    award(awards) // Correctly initialize 'award' instead of 'awards'
{
    //qDebug() << "SearchModel::SearchModel " ;
    dataProxy = awards->dataProxy;
    stationCallsignInHeader = true;
    setTable("log");
    setEditStrategy(QSqlTableModel::OnFieldChange);
    dxcc = -1;
    bandid = -1;
    modeid = -1;
    logn = -1;

    //award = new Awards(dataProxy, Q_FUNC_INFO);

    //qDebug() << "SearchModel::SearchModel: Rows obtained: " << QString::number(rowCount()) ;
    //qDebug() << "SearchModel::SearchModel - END" ;
}

SearchModel::~SearchModel()
{
    delete(dataProxy);
    delete(award);
}

void SearchModel::setDXCCColumn(const int _i)
{
   dxcc = _i;
}

void SearchModel::setBandIdColumn(const int _i)
{
   bandid = _i;
}

void SearchModel::setModeIdColumn(const int _i)
{
    modeid = _i;
}

void SearchModel::setLogNColumn(const int _i)
{
    logn = _i;
}

void SearchModel::createSearchModel(const int _i)
{
/*
    Log_Id = 0,
    Log_Name = 1,
    Log_BandId = 2,
    Log_ModeId = 3,
    Log_DateId = 4,
    Log_TimeId = 5

setRelation ( int column, const QSqlRelation & relation )

    model->setTable("employee");
    model->setRelation(2, QSqlRelation("city", "id", "name"));

The setRelation() call specifies that column 2 in table employee
is a foreign key that maps with field id of table city, and that
the view should present the city's name field to the user.

*/

/*
This should be coherent with the treeview
*/

      //qDebug() << "SearchModel::createSearchModel: log: " << QString::number(_i);

   //QString contestMode = dataProxy->getLogTypeOfUserLog(_i);


    QString stringQuery = QString("lognumber='%1'").arg(_i);
    QSqlQuery query(stringQuery);
    setFilter(stringQuery);
    //setColumnsToDX();
    select();
}

void SearchModel::setStationCallsignInHeader(const bool _s)
{
    stationCallsignInHeader = _s;
}

/*
 void SearchModel::setColumnsToDX()
 {
        //qDebug() << "SearchModel::setColumnsToDX" ;

     QSqlQuery q;

     //QString stringQuery = QString("SELECT call, qso_date, bandid, modeid, qsl_sent, qsl_rcvd, station_callsign, id FROM log LIMIT 1");
     QString stringQuery = QString("SELECT * FROM log _dateLIMIT 1");

     QSqlRecord rec; // = q.record();

     int nameCol;

     bool sqlOK = q.exec(stringQuery);
     if (!sqlOK)
     {
         emit queryError(Q_FUNC_INFO, q.lastError().databaseText(), q.lastError().nativeErrorCode(), q.lastQuery());

     }
    q.next();
    rec = q.record(); // Number of columns
    //qDebug() << "SearchModel::createSearchModel - query: " << q.lastQuery();
    //qDebug() << "SearchModel::createSearchModel - columns: " << QString::number(rec.count());

     nameCol = rec.indexOf("bandid");
     setRelation(nameCol, QSqlRelation("band", "id", "name"));

     nameCol = rec.indexOf("modeid");
     setRelation(nameCol, QSqlRelation("mode", "id", "submode"));

     nameCol = rec.indexOf("qso_date");
     setHeaderData(nameCol, Qt::Horizontal, tr("Date/Time"));

     nameCol = rec.indexOf("call");
     setHeaderData(nameCol, Qt::Horizontal,tr("Call"));

     nameCol = rec.indexOf("bandid");
     setHeaderData(nameCol, Qt::Horizontal, tr("Band"));

     nameCol = rec.indexOf("modeid");
     setHeaderData(nameCol, Qt::Horizontal, tr("Mode"));

     nameCol = rec.indexOf("qsl_sent");
     setHeaderData(nameCol, Qt::Horizontal, tr("QSL Sent"));

     nameCol = rec.indexOf("qsl_rcvd");
     setHeaderData(nameCol, Qt::Horizontal, tr("QSL Rcvd"));
    if (stationCallsignInHeader)
    {
        nameCol = rec.indexOf("station_callsign");
        setHeaderData(nameCol, Qt::Horizontal, tr("Station Callsign"));
    }

     nameCol = rec.indexOf("id");
     //setHeaderData(nameCol, Qt::Horizontal, tr("ID"));

     setSort(nameCol, Qt::AscendingOrder);
 }
*/

void SearchModel::setFilterString(const QString &_st)
 {
    //qDebug() << "SearchModel::setFilterString: " << _st;
    setFilter(_st);
    select();
    //qDebug() << "SearchModel::setFilterString: SelectStatement: " << selectStatement ();
 }

 void SearchModel::update()
 {
     select();
 }

 void SearchModel::setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default)
 {
        //qDebug() << "DXClusterWidget::setColors: " << _newOne << "/" << _needed << "/" << _worked << "/" << _confirmed << "/" << _default;
     // Just to pass the colors to the awards class
     award->setColors(_newOne,  _needed, _worked,  _confirmed, _default);
 }

QVariant SearchModel::data( const QModelIndex &index, int role ) const
 {
     if ( index.isValid() && role == Qt::ForegroundRole )
     {
         if ( index.column() == 2 )
         {
             EntityStatus _entityStatus;
            _entityStatus.dxcc  = index.siblingAtColumn(dxcc).data().toInt();
            _entityStatus.bandId    = index.siblingAtColumn(bandid).data().toInt();
            _entityStatus.modeId    = index.siblingAtColumn(modeid).data().toInt();
            _entityStatus.logId       = index.siblingAtColumn(logn).data().toInt();

            return QVariant( award->getQRZDXStatusColor(_entityStatus) );
            // return QVariant( QColor( Qt::red ) );
         }
         return QVariant( QColor( Qt::black ) );
     }

     return QSqlRelationalTableModel::data( index, role );
 }


